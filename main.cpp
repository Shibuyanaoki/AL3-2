#include "Audio.h"
#include "AxisIndicator.h"
#include "Clear.h"
#include "DirectXCommon.h"
#include "GameOver.h"
#include "GameScene.h"
#include "ImGuiManager.h"
#include "PrimitiveDrawer.h"
#include "Scene.h"
#include "TextureManager.h"
#include "Title.h"
#include "WinApp.h"

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	WinApp* win = nullptr;
	DirectXCommon* dxCommon = nullptr;
	// 汎用機能
	Input* input = nullptr;
	Audio* audio = nullptr;
	AxisIndicator* axisIndicator = nullptr;
	PrimitiveDrawer* primitiveDrawer = nullptr;
	GameScene* gameScene = nullptr;
	Title* title = nullptr;
	Clear* clear = nullptr;
	GameOver* gameOver = nullptr;

	Scene scene = Scene::TITLE;

	// ゲームウィンドウの作成
	win = WinApp::GetInstance();
	win->CreateGameWindow(L"LE2C_16_シブヤ_ナオキ_AL3");

	// DirectX初期化処理
	dxCommon = DirectXCommon::GetInstance();
	dxCommon->Initialize(win);

#pragma region 汎用機能初期化
	// ImGuiの初期化
	ImGuiManager* imguiManager = ImGuiManager::GetInstance();
	imguiManager->Initialize(win, dxCommon);

	// 入力の初期化
	input = Input::GetInstance();
	input->Initialize();

	// オーディオの初期化
	audio = Audio::GetInstance();
	audio->Initialize();

	// テクスチャマネージャの初期化
	TextureManager::GetInstance()->Initialize(dxCommon->GetDevice());
	TextureManager::Load("white1x1.png");

	// スプライト静的初期化
	Sprite::StaticInitialize(dxCommon->GetDevice(), WinApp::kWindowWidth, WinApp::kWindowHeight);

	// 3Dモデル静的初期化
	Model::StaticInitialize();

	// 軸方向表示初期化
	axisIndicator = AxisIndicator::GetInstance();
	axisIndicator->Initialize();

	primitiveDrawer = PrimitiveDrawer::GetInstance();
	primitiveDrawer->Initialize();
#pragma endregion

	// ゲームシーンの初期化
	gameScene = new GameScene();
	gameScene->Initialize();
	title = new Title;
	title->Initialize();
	clear = new Clear;
	clear->Initialize();
	gameOver = new GameOver;
	gameOver->Initialize();


	// メインループ
	while (true) {
		// メッセージ処理
		if (win->ProcessMessage()) {
			break;
		}

		// ImGui受付開始
		imguiManager->Begin();
		// 入力関連の毎フレーム処理
		input->Update();

		switch (scene) {
		case Scene::TITLE:
			if (title->IsSceneEnd() == true) {
				// 次のシーンを値を代入してシーン切り替え
				scene = title->NextScene();
				title->IsSceneStart();
			}

			// タイトルシーンの毎フレーム処理
			title->Update();

			break;

		case Scene::GAME:
			// ゲームシーンの毎フレーム処理
			gameScene->Update();

			if (gameScene->IsSceneEnd() == true) {
				// 次のシーンを値を代入してシーン切り替え
				scene = gameScene->NextSceneOver();
				//gameScene->IsSceneEndStart();
				gameScene->Reset();
			}
			
			if (gameScene->IsSceneClear() == true) {
				// 次のシーンを値を代入してシーン切り替え
				scene = gameScene->NextSceneClear();
				//gameScene->IsSceneClearStart();
				gameScene->Reset();
			}


			break;

		case Scene::GAMECLEAR:
			if (clear->IsSceneEnd() == true) {
				// 次のシーンを値を代入してシーン切り替え
				scene = clear->NextScene();
				clear->IsSceneStart();
			}

			// ゲームシーンの毎フレーム処理
			clear->Update();
			
			break;

		case Scene::GAMEOVER:
			if (gameOver->IsSceneEnd() == true) {
				// 次のシーンを値を代入してシーン切り替え
				scene = gameOver->NextScene();
				gameOver->IsSceneStart();				
			}

			// ゲームシーンの毎フレーム処理
			gameOver->Update();
		}

		// 軸表示の更新
		axisIndicator->Update();
		// ImGui受付終了
		imguiManager->End();

		// 描画開始
		dxCommon->PreDraw();

		switch (scene) {
		case Scene::TITLE:

			
			// ゲームシーンの描画
			title->Draw();
			

			break;

		case Scene::GAME:

			// ゲームシーンの描画
			gameScene->Draw();
			

			break;

		case Scene::GAMECLEAR:
			
			// ゲームシーンの描画
			clear->Draw();
			
			break;

		case Scene::GAMEOVER:
			
			// ゲームシーンの描画
			gameOver->Draw();
			
			break;
		}
		// 軸表示の描画
		axisIndicator->Draw();
		// プリミティブ描画のリセット
		primitiveDrawer->Reset();
		// ImGui描画
		imguiManager->Draw();
		// 描画終了
		dxCommon->PostDraw();

	}
	SafeDelete(gameScene);
	audio->Finalize();
	// ImGui解放
	imguiManager->Finalize();

	// ゲームウィンドウの破棄
	win->TerminateGameWindow();

	return 0;
}
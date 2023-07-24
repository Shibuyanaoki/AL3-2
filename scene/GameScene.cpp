#include "GameScene.h"
#include "AxisIndicator.h"
#include "TextureManager.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_, delete player_, delete debugCamera_, delete enemy_, delete modelSkydome_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// テクスチャ
	/*textureHandle_ = TextureManager::Load("uvChecker.png");
	sprite_ = Sprite::Create(textureHandle_, {100, 50});
	viewProjection_.Initialize();*/

	// モデル
	textureHandle_ = TextureManager::Load("uvChecker.png");
	model_ = Model::Create();

	//ビュープロジェクション
	viewProjection_.farZ = 100;
	viewProjection_.Initialize();

	// 3Dモデルの生成
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);

	// 敵の速度
	const float kEnemySpeed = 0.1f;
	// 敵の移動
	Vector3 velocity(kEnemySpeed, kEnemySpeed, -kEnemySpeed);
	// 敵のポジション
	Vector3 EnemyPosition = {8.0f, 1.0f, 40.0f};

	// 自キャラの生成
	player_ = new Player();
	// 自キャラの初期化
	player_->Initialize(model_, textureHandle_);
	// 敵の生成
	enemy_ = new Enemy;
	// 敵の初期化
	enemy_->Initialize(model_, EnemyPosition, velocity);

	//天球の生成
	skydome_ = new Skydome;

	//天球の初期化
	skydome_->Initialize(modelSkydome_);

	// デバックカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);

	enemy_->SetPlayer(player_);
}

void GameScene::Update() {
	// 自キャラの更新
	player_->Update();

	// 敵の更新
	enemy_->Update();

	// デバッグカメラの更新
	debugCamera_->Update();

	//天球の更新
	skydome_->Update();

#ifdef _DEBUG
	if (input_->TriggerKey(DIK_1) && isDebugCameraActive_ == 0) {
		isDebugCameraActive_ = 1;
	} else if (input_->TriggerKey(DIK_1) && isDebugCameraActive_ == 1) {
		isDebugCameraActive_ = 0;
	}
#endif

	// カメラの処理
	if (isDebugCameraActive_ == 1) {
		// デバックカメラの更新
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		// ビュープロジェクション行列の転送
		viewProjection_.TransferMatrix();
	} else {
		// ビュープロジェクション行列の更新と転送
		viewProjection_.UpdateMatrix();
	}

	// 軸方向の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	// 軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

	CheckAllCollision();
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	player_->Draw(viewProjection_);

	// 敵の描画
	enemy_->Draw(viewProjection_);

	//天球の描画 
	skydome_->Draw(viewProjection_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::CheckAllCollision() {
	// 判定対象AとBの座標
	Vector3 posA, posB;
	// 自弾リストを取得
	const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();
	// 敵弾リストの取得
	const std::list<EnemyBullet*>& enemyBullets = enemy_->GetBullet();

#pragma region
	// 自キャラと敵弾の当たり判定

	// 自キャラの座標
	posA = player_->GetWorldPosition();

	// 自キャラと敵弾全ての当たり判定
	for (EnemyBullet* bullet : enemyBullets) {
		// 敵弾の座標
		posB = bullet->GetWorldPosition();

		float Hit = (posA.x - posB.x) * (posA.x - posB.x) + (posA.y - posB.y) * (posA.y - posB.y) +
		            (posA.z - posB.z) * (posA.z - posB.z);

		float Radius = (player_->radius + bullet->radius) * (player_->radius + bullet->radius);

		if (Hit <= Radius) {
			// 自キャラの衝突自コールバックを呼び出す
			player_->OnCollision();
			// 敵弾の衝突時コールバックを呼び出す
			bullet->OnCollision();
		}
	}

#pragma endregion

#pragma region
	// 自弾と敵キャラの当たり判定

	// 敵の判定
	posA = enemy_->GetWorldPosition();

	for (PlayerBullet* bullet : playerBullets) {
		// 自弾の座標
		posB = bullet->GetWorldPosition();

		float Hit = (posA.x - posB.x) * (posA.x - posB.x) + (posA.y - posB.y) * (posA.y - posB.y) +
		            (posA.z - posB.z) * (posA.z - posB.z);

		float Radius = (bullet->radius + enemy_->radius) * (bullet->radius + enemy_->radius);

		if (Hit <= Radius) {
			// 敵の衝突自コールバックを呼び出す
			enemy_->OnCollision();
			// 自弾の衝突時コールバックを呼び出す
			bullet->OnCollision();
		}
	}

#pragma endregion

#pragma
	// 自弾と敵弾の当たり判定

	// 敵の判定
	// posA = enemy_->GetWorldPosition();

	for (PlayerBullet* playerBullet : playerBullets) {
		posA = playerBullet->GetWorldPosition();
		for (EnemyBullet* enemyBullet : enemyBullets) {
			// 自弾の座標
			posB = enemyBullet->GetWorldPosition();

			float Hit = (posA.x - posB.x) * (posA.x - posB.x) +
			            (posA.y - posB.y) * (posA.y - posB.y) +
			            (posA.z - posB.z) * (posA.z - posB.z);

			float Radius = (playerBullet->radius + enemyBullet->radius) *
			               (playerBullet->radius + enemyBullet->radius);

			if (Hit <= Radius) {
				// 自弾の衝突自コールバックを呼び出す
				playerBullet->OnCollision();
				// 敵弾の衝突時コールバックを呼び出す
				enemyBullet->OnCollision();
			}
		}
	}

#pragma endregion

}
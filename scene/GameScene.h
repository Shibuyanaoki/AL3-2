#pragma once

#include "Audio.h"
#include "DebugCamera.h"
#include "DirectXCommon.h"
#include "Enemy.h"
#include "Input.h"
#include "Model.h"
#include "Player.h"
#include "RailCamera.h"
#include "SafeDelete.h"
#include "Scene.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "skydome.h"
#include <sstream>

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {
public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 敵弾の追加する
	/// </summary>
	/// <param name="enemyBullet"></param>
	void AddEnemyBullet(EnemyBullet* enemyBullet);

	/// <summary>
	/// 敵発生データの読み込み
	/// </summary>
	void LoadEnemyPopData();

	/// <summary>
	/// 敵発生コマンドの更新
	/// </summary>
	void UpdeteEnemyPopCommand();

	bool IsSceneClear() { return isSceneClear; }
	bool IsSceneEnd() { return isSceneEnd; }
	Scene NextSceneClear() { return Scene::GAMECLEAR; }
	Scene NextSceneOver() { return Scene::GAMEOVER; }
	bool IsSceneClearStart() { return isSceneClear = false; }
	bool IsSceneEndStart() { return isSceneEnd = false; }

	void EnemyOccurrence(Vector3 position, Vector3 velocity);

	void Reset();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	uint32_t textureHandle_ = 0;
	//uint32_t textureHandleHP_ = 0;
	uint32_t textureHandleModel_ = 0;
	float inputFloat3[3] = {0, 0, 0};
	void CheckAllCollision();

	Sprite* sprite_ = nullptr;
	Sprite* sprite2_ = nullptr;
	Sprite* sprite3_ = nullptr;
	Model* model_ = nullptr;
	Model* modelSkydome_ = nullptr;

	Player* player_ = nullptr;
	ViewProjection viewProjection_;
	Enemy* enemy_ = nullptr;
	Skydome* skydome_ = nullptr;
	RailCamera* railCamera_ = nullptr;
	EnemyBullet* enemyBullet_ = nullptr;
	bool isSceneClear = false;
	bool isSceneEnd = false;
	int timer = 0;

	// 弾
	std::list<EnemyBullet*> enemyBullets_;
	// 敵
	std::list<Enemy*> enemys_;
	// 敵発生コマンド
	std::stringstream enemyPopCommands;

	int standFlag = false;
	int standTime = 0;
	int HP = 3;

	/// <summary>
	/// ゲームシーン用
	/// </summary>

	// デバックカメラ有効
	bool isDebugCameraActive_ = false;

	// デバックカメラ
	DebugCamera* debugCamera_ = nullptr;
};
#pragma once
#include "EnemyBullet.h"
#include "Input.h"
#include "Model.h"
#include "WorldTransform.h"
#include <list>

/// <summary>
/// 敵
/// </summary>
class Enemy {
public:
	~Enemy();

	void Initialize(Model* model, Vector3& position, const Vector3& velocity);

	void Update();

	void Draw(ViewProjection& viewProjection);

private:
	void Approach();

	void Leave();

	void Fire();

private:
	// 行動フェーズ
	enum class Phase {
		Approach, // 接近する
		Leave,    // 離脱する
	};

	// 弾
	std::list<EnemyBullet*> enemyBullets_;

	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t texturehandle_ = 0u;
	// 速度
	Vector3 velocity_;
	// フェーズ
	Phase phase_ = Phase::Approach;
	EnemyBullet* enemyBullet_ = nullptr;
};

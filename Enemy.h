#pragma once
#include "Input.h"
#include "Model.h"
#include "WorldTransform.h"
#include "PlayerBullet.h"
#include <list>


/// <summary>
/// 敵
/// </summary>
class Enemy {
public:

	void Initialize( Model* model,  Vector3& position, const Vector3& velocity);

	void Update();

	void Draw(ViewProjection& viewProjection);

	void Approach();

	void Leave();

private:
	// 行動フェーズ
	enum class Phase {
		Approach, // 接近する
		Leave,    // 離脱する
	};

	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t texturehandle_ = 0u;
	// 速度
	Vector3 velocity_;
	//フェーズ
	Phase phase_ = Phase::Approach;	

};

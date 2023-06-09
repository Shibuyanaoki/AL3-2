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

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t texturehandle_ = 0u;
	// 速度
	Vector3 velocity_;

};

#include "Enemy.h"

void Enemy::Initialize(Model* model, Vector3& position, const Vector3& velocity) {
	assert(model);

	model_ = model;
	texturehandle_ = TextureManager::Load("tex1.png");

	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	// 引数で受け取った初期座標をセット
	worldTransform_.translation_ = position;
	// 引数で受け取った速度をメンバ変数に代入
	velocity_ = velocity;
}

void Enemy::Update() {
	worldTransform_.matWorld_ = MakeAffineMatrix(
	    worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	// 行列の転送　行列の計算後に行う
	worldTransform_.TransferMatrix();

	switch (phase_) {
	case Phase::Approach:
	default:
		
		Approach();

		break;
	case Phase::Leave:
	
	Leave();

		break;
	}
};

void Enemy::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, texturehandle_);
};

void Enemy::Approach() {
	// 移動(ベクトルを加算)
	worldTransform_.translation_.z += velocity_.z;

	if (worldTransform_.translation_.z < 0.0f) {
		phase_ = Phase::Leave;
	}

}

void Enemy::Leave() {
	// 移動(ベクトルを加算)

	worldTransform_.translation_.x -= velocity_.x;
	worldTransform_.translation_.y += velocity_.y;
	worldTransform_.translation_.z += velocity_.z;

}


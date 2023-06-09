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

void Enemy::Update(){
	worldTransform_.matWorld_ = MakeAffineMatrix(
	    worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	// 行列の転送　行列の計算後に行う
	worldTransform_.TransferMatrix();

	worldTransform_.translation_.x += velocity_.x;
	worldTransform_.translation_.y += velocity_.y;
	worldTransform_.translation_.z -= velocity_.z;

};


void Enemy::Draw(ViewProjection& viewProjection) {
	model_ -> Draw(worldTransform_, viewProjection, texturehandle_);
};
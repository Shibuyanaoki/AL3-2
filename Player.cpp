#include "Player.h"
#include "ImGuiManager.h"
#include "MT.h"
#include "iostream"
#include <cassert>

using namespace std;

void Player::Initialize(Model* model, uint32_t textureHandle) {
	assert(model);

	model_ = model;
	texturehandle_ = textureHandle;

	worldTransform_.Initialize();

	input_ = Input::GetInstance();

	// X,Y,Z方向のスケーリングを設定
	worldTransform_.scale_ = {5.0f, 1.0f, 1.0f};

	// X,Y,Z方向の回転を設定
	worldTransform_.rotation_ = {0.0f, 0.0f, 0.0f};

	//// X,Y,Z方向の平行移動を設定
	worldTransform_.translation_ = {0.0f, 0.0f, 0.0f};

	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
}

void Player::Update() {
	worldTransform_.TransferMatrix();

	// デスフラグの立った弾を削除
	bullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});


	// キャラクターの移動ベクトル
	Vector3 move = {0, 0, 0};

	// キャラクターの移動速さ
	const float kCharacterSpeed = 0.2f;

	// 押した方向で移動ベクトルを変更(左右)
	if (input_->PushKey(DIK_LEFT)) {
		move.x -= kCharacterSpeed;
	} else if (input_->PushKey(DIK_RIGHT)) {
		move.x += kCharacterSpeed;
	}

	// 押した方向でベクトルを変更(上下)

	if (input_->PushKey(DIK_UP)) {
		move.y += kCharacterSpeed;
	} else if (input_->PushKey(DIK_DOWN)) {
		move.y -= kCharacterSpeed;
	}

	// 移動限界座標
	const float kMoveLimitX = 31;
	const float kMoveLimitY = 19;

	// 範囲を超えない処理
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);

	// 座標飯銅(ベクトルの加算)
	worldTransform_.translation_.x += move.x;
	worldTransform_.translation_.y += move.y;
	worldTransform_.translation_.z += move.z;

	worldTransform_.matWorld_ = MakeAffineMatrix(
	    worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	// 行列の転送　行列の計算後に行う
	worldTransform_.TransferMatrix();

	// キャラクターの座標を画面表示する処理
	ImGui::Begin("Debug1");
	float playerPos[] = {
	    worldTransform_.translation_.x, worldTransform_.translation_.y,
	    worldTransform_.translation_.z};
	ImGui::SliderFloat3("PlayerPos", playerPos, 0, 1280);

	// 処理のままだとSilderFloat3でplayerPosの値を変えているので実際の座標(translation)が
	// 変わっていないのでここで変更する
	worldTransform_.translation_.x = playerPos[0];
	worldTransform_.translation_.y = playerPos[1];
	worldTransform_.translation_.z = playerPos[2];

	ImGui::End();

	Rotate();

	Attack();

	// 弾の更新
	/*if (bullet_) {
	    bullet_->Update();
	}*/

	for (PlayerBullet* bullet : bullets_) {
		bullet->Update();
	}

}

// bullet_の解放

Player::~Player() {
	for (PlayerBullet* bullet : bullets_) {
		delete bullet;
	}
}

void Player::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, texturehandle_);

	// 弾の描画
	/*if (bullet_) {
	    bullet_->Draw(viewProjection);
	}*/

	for (PlayerBullet* bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}

void Player::Rotate() {
	// 回転速さ[ラジアン/frame]
	const float kRotSpeed = 0.02f;

	// 押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_A)) {
		worldTransform_.rotation_.y -= kRotSpeed;
	} else if (input_->PushKey(DIK_D)) {
		worldTransform_.rotation_.y += kRotSpeed;
	}
}

void Player::Attack() {

	if (input_->TriggerKey(DIK_SPACE)) {

		// 弾の速度
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		// 速度ベクトルを時機向きに合わせて回転させる
		velocity = TransformNormal(velocity, worldTransform_.matWorld_);

		// 弾を生成し、初期化
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(model_, worldTransform_.translation_, velocity);

		// 弾を登録する
		bullets_.push_back(newBullet);
	}
}

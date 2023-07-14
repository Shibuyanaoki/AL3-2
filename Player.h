#include "Input.h"
#include "Model.h"
#include "PlayerBullet.h"
#include "WorldTransform.h"
#include <list>

class Player {

public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Player();

	void Initialize(Model* model, uint32_t textureHandle);

	void Update();

	void Draw(ViewProjection& viewProjection);

	// ワールド座標を取得
	Vector3 GetWorldPosition();
	const std::list<PlayerBullet*>& GetBullets() const { return bullets_; }

	void OnCollision();

public:
	float radius = 3;

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t texturehandle_ = 0u;
	Input* input_ = nullptr;
	float inputFloat3[3] = {0, 0, 0};
	// PlayerBullet* bullet_ = nullptr;
	//  弾
	std::list<PlayerBullet*> bullets_;

private:
	void Rotate();
	void Attack();
};
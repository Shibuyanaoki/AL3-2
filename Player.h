#include "Input.h"
#include "Model.h"
#include "WorldTransform.h"
#include "PlayerBullet.h"
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

	//弾
	std::list<PlayerBullet*> bullets_;

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t texturehandle_ = 0u;
	Input* input_ = nullptr;
	float inputFloat3[3] = {0, 0, 0};
	PlayerBullet* bullet_ = nullptr;
	

private:
	void Rotate();
	void Attack();
	//ワールド座標を取得
	Vector3 GetWorldPosition();

};
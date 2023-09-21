﻿#pragma once
#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Scene.h"

class Title {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	Title();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Title();

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
	/// 
	/// </summary>
	/// <returns></returns>
	bool IsSceneEnd() { return isSceneEnd; }

	bool IsSceneStart() { return isSceneEnd = false; }

	Scene NextScene() { return Scene::GAME; }

private:
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	bool isSceneEnd = false;
	uint32_t textureHandle_ = 0;
	Sprite* sprite_ = nullptr;

};

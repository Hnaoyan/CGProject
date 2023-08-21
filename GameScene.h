#pragma once

#include "DirectXCommon.h"
#include "Sprite.h"
#include "Input.h"
#include "Audio.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Model.h"
#include <memory>

class GameScene
{
public:

	GameScene() {};

	~GameScene();

	void Initialize();

	void Update();

	void Draw();

private:
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	Sprite* sprite_ = nullptr;
	
	uint32_t sound_;

	ViewProjection viewProjection_;

	WorldTransform cubeWVP_;

	Model* cubeModel_ = nullptr;

	int32_t tex = 0;

	// それぞれの座標系
	Sprite::Transform triangle1 = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };

	// スプライト
	Sprite::Transform spriteTransform = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	Sprite::Transform spriteUvTransform = {
		{1.0f,1.0f,1.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f},
	};

	// 球
	Sprite::Transform SphereTransform = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,10.0f} };
	
	// 三角用のカメラ
	Sprite::Transform cameraTransform{ {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,-5.0f} };

	/// <summary>
	/// 表示するかどうか
	/// </summary>
	bool IsTriangel = 1;

	bool IsSprite = 0;

	bool IsSphere = 0;

	/// 画像の色
	Vector4 color_ = { 1.0f,1.0f,1.0f,1.0f };

	/// <summary>
	/// ライティング関係
	/// </summary>
	Sprite::DirectionalLight lighting_;

	int lightPattern = Sprite::None;
};


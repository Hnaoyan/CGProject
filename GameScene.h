#pragma once

#include "DirectXCommon.h"
#include "Sprite.h"

class GameScene
{
public:

	GameScene() {};

	~GameScene() {};

	void Initialize();

	void Update();

	void Draw();

private:
	DirectXCommon* dxCommon_ = nullptr;

	Sprite* sprite_ = nullptr;
	
	// それぞれの座標系
	Transform triangle1 = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };

	Transform spriteTransform = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };

	Transform SphereTransform = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	
	// 三角用のカメラ
	Transform cameraTransform{ {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,-5.0f} };

	// 球用のカメラ
	Transform cameraSpTransform{ {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,-10.0f} };

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


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

	Transform triangle1 = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };

	Transform cameraTransform{ {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,-5.0f} };

	bool IsTriangel = 1;

	Transform spriteTransform = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };

};


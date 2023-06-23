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

};


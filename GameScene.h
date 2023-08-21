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

};


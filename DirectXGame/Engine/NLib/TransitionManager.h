#pragma once
#include "Sprite.h"
#include "NBase/Singleton.h"
#include "DirectXCommon.h"
#include <memory>

class TransitionManager : public Singleton<TransitionManager>
{
public:
	void Initialize();
	void Start() {
		isStart_ = true;
	}
	void End();
	void Update();
	void Draw();

	bool ChangeTiming() {
		return !isStart_ && isEnd_;
	}

private:
	std::unique_ptr<Sprite> blackSprite_;
	Vector4 color_;
	DirectXCommon* dxCommon_ = nullptr;

	float changeFrame_;

	bool isStart_;
	bool isEnd_;

};


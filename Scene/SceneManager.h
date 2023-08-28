#pragma once
#include <memory>
#include "BaseScene.h"
#include "GameScene.h"

class SceneManager {
private:
	std::unique_ptr<BaseScene> sceneArray_[3];

	int sceneNum_;
	int prevSceneNum_;

public:
	SceneManager();
	~SceneManager();


	void Update();

	void Draw();

};

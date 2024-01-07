#pragma once
#include <memory>
#include "IScene.h"
#include "TestScene.h"
#include "GameScene.h"
#include "TitleScene.h"

enum Scene {
	TITLE, GAMESCENE, CLEAR,
};

class SceneManager {
private:
	std::unique_ptr<IScene> sceneArray_[3];

	int sceneNum_;
	int prevSceneNum_;

public:
	SceneManager();
	~SceneManager();


	void Update();

	void Draw();

};

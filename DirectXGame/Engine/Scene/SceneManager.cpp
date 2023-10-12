#include "SceneManager.h"

SceneManager::SceneManager() 
{ 
	sceneArray_[TITLE] = std::make_unique<TitleScene>();
	sceneArray_[GAMESCENE] = std::make_unique<GameScene>();
	sceneArray_[CLEAR] = std::make_unique<GameScene>();

	sceneNum_ = GAMESCENE;
	sceneArray_[sceneNum_]->Initialize();
}

SceneManager::~SceneManager() {}

void SceneManager::Update() 
{ 
	prevSceneNum_ = this->sceneNum_;
	sceneNum_ = sceneArray_[sceneNum_]->GetSceneNum();

	if (prevSceneNum_ != sceneNum_) {
		sceneArray_[sceneNum_]->Initialize();
	}

	sceneArray_[sceneNum_]->Update();

}

void SceneManager::Draw() 
{ sceneArray_[sceneNum_]->Draw(); }

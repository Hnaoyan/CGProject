#include "SceneManager.h"
#include "TextureManager.h"

SceneManager::SceneManager() 
{ 
	sceneArray_[TITLE] = std::make_unique<TitleScene>();
	sceneArray_[GAMESCENE] = std::make_unique<GameScene>();
	sceneArray_[CLEAR] = std::make_unique<ClearScene>();

	sceneNum_ = GAMESCENE;
	sceneArray_[sceneNum_]->Initialize();
	//AudioManager* instance = AudioManager::GetInstance();
	//instance->PlayAudio(instance->GetSound("BGM"), true, 0.5f);

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
{ 

	sceneArray_[sceneNum_]->Draw();

}

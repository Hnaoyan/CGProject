#include "SceneManager.h"
#include <assert.h>

SceneManager::SceneManager() 
{ 
	//ChangeScene("TITLE");

	//sceneArray_[TITLE] = std::make_unique<TestScene>();
	//sceneArray_[GAMESCENE] = std::make_unique<GameScene>();
	//sceneArray_[CLEAR] = std::make_unique<GameScene>();

	//sceneNum_ = GAMESCENE;
	//sceneArray_[sceneNum_]->Initialize();
}

SceneManager::~SceneManager() 
{
	delete nowScene_;
}

void SceneManager::Update() 
{ 
	//prevSceneNum_ = this->sceneNum_;
	//sceneNum_ = sceneArray_[sceneNum_]->GetSceneNum();

	//if (prevSceneNum_ != sceneNum_) {
	//	sceneArray_[sceneNum_]->Initialize();
	//}

	//sceneArray_[sceneNum_]->Update();

	if (nextScene_) {
		if (nowScene_) {
			delete nowScene_;
		}
		// シーン切り替え
		nowScene_ = nextScene_;
		nextScene_ = nullptr;
		// 次のシーンの初期化
		nowScene_->Initialize();
	}
	nowScene_->Update();
}

void SceneManager::Draw() 
{ 
	nowScene_->Draw();
	//sceneArray_[sceneNum_]->Draw();
}

void SceneManager::ChangeScene(const std::string & sceneName)
{
	assert(sceneFactory_);
	assert(nextScene_ == nullptr);

	// 次のシーン生成
	nextScene_ = sceneFactory_->CreateScene(sceneName);

}

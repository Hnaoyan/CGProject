#include "SceneManager.h"
#include <assert.h>

SceneManager::SceneManager() 
{ 
	transitionManager_ = std::make_unique<TransitionManager>();
}

SceneManager::~SceneManager() 
{
	delete nowScene_;
}

void SceneManager::Update() 
{ 
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
}

void SceneManager::ChangeScene(const std::string & sceneName)
{
	assert(sceneFactory_);
	assert(nextScene_ == nullptr);

	// 次のシーン生成
	nextScene_ = sceneFactory_->CreateScene(sceneName);

}

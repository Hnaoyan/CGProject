#pragma once
#include <memory>
#include "IScene.h"
#include "SceneList.h"
#include "AbstractSceneFactory.h"
#include "TransitionManager.h"

class SceneManager {
private:
	std::unique_ptr<IScene> sceneArray_[3];

	int sceneNum_;
	int prevSceneNum_;

	/// <summary>
	/// シーンファクトリー
	/// </summary>
	AbstractSceneFactory* sceneFactory_ = nullptr;

	// 現在のシーン
	IScene* nowScene_ = nullptr;
	// 次のシーン
	IScene* nextScene_ = nullptr;

	TransitionManager* transitionManager_;

	std::string nextSceneName_;

public:
	SceneManager();
	~SceneManager();

	static SceneManager* GetInstance() {
		static SceneManager instance;
		return &instance;
	}

	void SetSceneFactory(AbstractSceneFactory* sceneFactory) { sceneFactory_ = sceneFactory; }

	//void Initialize();

	void Update();

	void Draw();

	void ChangeScene(const std::string& sceneName);

};

#include "SceneFactory.h"


IScene* SceneFactory::CreateScene(const std::string& sceneName)
{
	// 次のシーン
	IScene* newScene = nullptr;

	if (sceneName == "TITLE") {
		newScene = new TestScene();
	}

	else if (sceneName == "GAMESCENE") {
		newScene = new GameScene();
	}

	return newScene;
}

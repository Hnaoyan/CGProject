#include "SceneFactory.h"


IScene* SceneFactory::CreateScene(const std::string& sceneName)
{
	// 次のシーン
	IScene* newScene = nullptr;

	if (sceneName == "TITLE") {
		newScene = new TitleScene();
	}

	else if (sceneName == "GAME") {
		newScene = new GameScene();
	}

	else if (sceneName == "TEST") {
		newScene = new TestScene();
	}

	else if (sceneName == "CLEAR") {
		newScene = new ClearScene();
	}

	return newScene;
}

#include "IScene.h"
#include "SceneManager.h"

int IScene::sceneNum = TITLE;

void IScene::Initialize()
{
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	viewProjection_.Initialize();
}

#include "DirectXCommon.h"
#include"WinApp.h"
#include "Sprite.h"
#include "GameScene.h"

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	WinApp* winApp;
	DirectXCommon* dxCommon;
	GameScene* gameScene = nullptr;

	winApp = WinApp::GetInstance();
	winApp->CreateGameWindow();

	dxCommon = DirectXCommon::GetInstance();
	dxCommon->Initialize(winApp);

	Sprite::StaticInitialize(dxCommon->GetDevice());

	gameScene = new GameScene();
	gameScene->Initialize();

	while (winApp->ProcessMessage() == 0)
	{
		gameScene->Update();

		// 描画前処理
		dxCommon->PreDraw();

		gameScene->Draw();
		// 描画後処理
		dxCommon->PostDraw();
	}

	CoUninitialize();
}

#include "DirectXCommon.h"
#include"WinApp.h"
#include "Sprite.h"
#include "GameScene.h"
#include "ImGuiManager.h"

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	WinApp* winApp;
	DirectXCommon* dxCommon;
	GameScene* gameScene = nullptr;
	ImGuiManager* imguiManager = nullptr;

	winApp = WinApp::GetInstance();
	winApp->CreateGameWindow();

	dxCommon = DirectXCommon::GetInstance();
	dxCommon->Initialize(winApp);

	imguiManager = ImGuiManager::GetInstance();
	imguiManager->Initialize(dxCommon, winApp);

	Sprite::StaticInitialize(dxCommon->GetDevice());

	gameScene = new GameScene();
	gameScene->Initialize();

	while (winApp->ProcessMessage() == 0)
	{
		// ImGui受付開始
		imguiManager->Begin();
		// ゲームシーン更新処理
		gameScene->Update();

		// 描画前処理
		dxCommon->PreDraw();

		// ゲームシーン描画処理
		gameScene->Draw();

		// ImGui受付終了
		imguiManager->End();

		// ImGui描画
		imguiManager->Draw();

		// 描画後処理
		dxCommon->PostDraw();
	}

	imguiManager->Finalize();
	CoUninitialize();
}

#include "DirectXCommon.h"
#include"WinApp.h"
#include "Sprite.h"
#include"GameScene.h"
#include "ImGuiManager.h"
#include"TextureManager.h"
#include "Audio.h"
#include"Input.h"
#include "Model.h"

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	WinApp* winApp;
	DirectXCommon* dxCommon;
	Audio* audio;
	Input* input;
	GameScene* gameScene = nullptr;
	ImGuiManager* imguiManager = nullptr;

	// WindowsApp
	winApp = WinApp::GetInstance();
	winApp->CreateGameWindow();

	// DirectX
	dxCommon = DirectXCommon::GetInstance();
	dxCommon->Initialize(winApp);

	// Input
	input = Input::GetInstance();
	input->Initialize();

	// オーディオ
	audio = Audio::GetInstance();
	audio->Initialize();


	// Imgui
	imguiManager = ImGuiManager::GetInstance();
	imguiManager->Initialize(dxCommon, winApp);

	// テクスチャマネージャの初期化
	TextureManager::GetInstance()->Initialize(dxCommon->GetDevice());
	TextureManager::Load("white1x1.png");

	Sprite::StaticInitialize(dxCommon->GetDevice());

	Model::StaticInitialize();

	gameScene = new GameScene();
	gameScene->Initialize();

	while (winApp->ProcessMessage() == 0)
	{
		if (winApp->ProcessMessage()) {
			break;
		}

		// 入力処理
		input->Update();
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
	audio->Finalize();
	CoUninitialize();

	
	return 0;
}

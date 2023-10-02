#include "Framework.h"

void Framework::Execute()
{
	// 初期化
	Initialize();

	// ゲームループ
	while (true)
	{
		// 更新処理
		Update();

		// 終了確認
		if (endRequest_) {
			break;
		}

		// 描画処理
		Draw();
	}

	// 終了処理
	Finalize();
}

void Framework::Initialize()
{
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

	// スプライトインスタンス
	Sprite::StaticInitialize(dxCommon->GetDevice(), (int)WinApp::kClientWidth, (int)WinApp::kClientHeight);

	// モデルインスタンス
	Model::StaticInitialize();

}

void Framework::Finalize()
{
	imguiManager->Finalize();
	audio->Finalize();
	CoUninitialize();
}

void Framework::Update()
{

}

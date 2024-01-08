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
	winApp = WindowAPI::GetInstance();
	winApp->CreateGameWindow();

	// Heap初期化
	descriptorManager = DescriptorManager::GetInstance();
	descriptorManager->StaticInitialize();
	// DirectX
	dxCommon = DirectXCommon::GetInstance();
	dxCommon->Initialize(winApp);

	// モデル用シェーダー初期化
	Shader::Initialize();
	// モデル用パイプラインの初期化
	PipelineManager::CreatePipeline();

	// Input
	input = Input::GetInstance();
	input->Initialize();

	// オーディオ
	audio = Audio::GetInstance();
	audio->Initialize();

	audioManager_ = std::make_unique<AudioManager>();

	// Imgui
	imguiManager = ImGuiManager::GetInstance();
	imguiManager->Initialize(dxCommon, winApp);

	// テクスチャマネージャの初期化
	TextureManager::GetInstance()->Initialize(dxCommon);
	TextureManager::Load("white1x1.png");


	// スプライトインスタンス
	Sprite::StaticInitialize(dxCommon->GetDevice(), (int)WindowAPI::kClientWidth, (int)WindowAPI::kClientHeight);

	// モデルインスタンス
	Model::StaticInitialize();

	// パーティクルインスタンス
	// 未完成
	//Particle::GetInstance()->StaticInitialize();
	NEngine::GetInstance()->Initialize(dxCommon, winApp);

}

void Framework::Finalize()
{
	imguiManager->Finalize();
	descriptorManager->Finalize();
	audio->Finalize();
	CoUninitialize();
}

void Framework::Update()
{

}

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
	descriptorManager_ = DescriptorManager::GetInstance();
	descriptorManager_->StaticInitialize();
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

	audioManager_ = AudioManager::GetInstance();
	uint32_t bgm = audio->LoadWave("bgm1.wav");
	audioManager_->AddSound(bgm, "BGM");

	// Imgui
	imguiManager_ = ImGuiManager::GetInstance();
	imguiManager_->Initialize(dxCommon, winApp);

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
	//NEngine::GetInstance()->Initialize(dxCommon, winApp);

}

void Framework::Finalize()
{
	SafeDelete(sceneManager_);
	imguiManager_->Finalize();
	descriptorManager_->Finalize();
	audio->Finalize();
	CoUninitialize();
}

void Framework::Update()
{

}

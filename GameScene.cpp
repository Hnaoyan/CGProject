#include "GameScene.h"
#include "ImGuiManager.h"
#include "externals/imgui/imgui_impl_dx12.h"
#include "externals/imgui/imgui_impl_win32.h"
#include "Camera.h"
#include "TextureManager.h"
#include "Model.h"


GameScene::~GameScene() {
	delete cubeModel_;
}

void GameScene::Initialize() {
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	sound_ = audio_->LoadWave("fanfare.wav");

	viewProjection_.Initialize();

	tex = TextureManager::Load("uvChecker.png");
	sprite_ = Sprite::Create(tex, { 0,0 }, { 1.0f,1.0f,1.0f,1.0f }, { 0,0 }, 0, 0);
	cubeWVP_.Initialize();

	//cubeModel_ = Model::Create();

}

void GameScene::Update() {
	if (input_->TrigerKey(DIK_SPACE)) {
		audio_->PlayWave(sound_, 0, 0.45f);
	}
	
}

void GameScene::Draw() {

	// コマンドリストを取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();
	// 描画前
	Sprite::PreDraw(commandList);

	sprite_->Draw();
	// 描画後
	Sprite::PostDraw();

	Model::PreDraw(commandList);

	//cubeModel_->Draw(cubeWVP_, viewProjection_, tex);

	Model::PostDraw();

}

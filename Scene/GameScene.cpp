#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	viewProjection_.Initialize();

	// 追従カメラ
	followCamera_ = std::make_unique<FollowCamera>();
	followCamera_->Initialize();

	baseCamera_ = std::make_unique<BaseCamera>();
	baseCamera_->Initialize();

	baseCamera_->SetPosition({ 0, 10.0f, -25.0f });
	baseCamera_->SetRotation({ 0.3f, 0, 0 });

	baseWorld_.Initialize();
	baseWorld_.translation_ = { 0,0,0 };
	baseWorld_.scale_ = { 1.0f,1.0f,1.0f };

	model_.reset(Model::Create());

	for (int i = 0; i < 3; i++) {
		worl_[i].Initialize();
		worl_[i].translation_ = { 10.0f * i,0,0 };
	}

	soundFan_ = audio_->LoadWave("fanfare.wav");

	//tex_ = TextureManager::Load("white1x1.png");

}

void GameScene::Update()
{

	if (input_->TriggerKey(DIK_A)) {
		audio_->PlayWave(soundFan_);
	}

	baseWorld_.UpdateMatrix();

	for (int i = 0; i < 3; i++) {
		worl_[i].UpdateMatrix();
	}

	/// 当たり判定（仮
	CheckAllCollision();

	/// カメラ関係の更新処理
	CameraUpdate();

}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	model_->Draw(baseWorld_, viewProjection_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::CheckAllCollision()
{

}

void GameScene::CameraUpdate()
{
#ifdef _DEBUG
	if (input_->TriggerKey(DIK_L)) {
		if (isDebug_) {
			isDebug_ = false;
		}
		else {
			isDebug_ = true;
		}
	}
#endif // DEBUG

	baseCamera_->Update();

	// デバックカメラか追尾カメラ
	if (isDebug_) {

	}
	else {
		//viewProjection_.matView = followCamera_->GetView().matView;
		//viewProjection_.matProjection = followCamera_->GetView().matProjection;
		//viewProjection_.TransferMatrix();
		viewProjection_.matView = baseCamera_->GetView().matView;
		viewProjection_.matProjection = baseCamera_->GetView().matProjection;
		viewProjection_.TransferMatrix();
	}


}

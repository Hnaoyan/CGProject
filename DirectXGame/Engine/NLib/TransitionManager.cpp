#include "TransitionManager.h"
#include "TextureManager.h"
#include "imgui.h"

void TransitionManager::Initialize()
{
	dxCommon_ = DirectXCommon::GetInstance();
	uint32_t texture = TextureManager::Load("white1x1.png");
	color_ = { 1,1,1,0 };
	blackSprite_.reset(Sprite::Create(texture, {}, color_, { 0.5f,0.5f }, false, false));
	blackSprite_->SetPosition({ 640,360 });
	blackSprite_->SetSize({ 1280,720 });
	changeFrame_ = 150.0f;
}

void TransitionManager::Update()
{
	ImGui::Begin("Transi");
	ImGui::DragFloat4("co", &color_.x, 0.01f);
	ImGui::End();
	blackSprite_->SetColor(color_);

	if (isStart_ && !isEnd_) {
		color_.w += (1.0f / changeFrame_);
		if (color_.w > 1.0f) {
			color_.w = 1.f;
			isStart_ = false;
			isEnd_ = true;
		}
	}
	if (isEnd_ && !isStart_) {
		color_.w -= (1.0f / changeFrame_);
		if (color_.w < 0.0f) {
			color_.w = 0.f;
			isEnd_ = false;
		}
	}


}

void TransitionManager::Draw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 描画前処理
	Sprite::PreDraw(commandList);

	// 描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

	// 描画前処理
	Sprite::PreDraw(commandList);

	blackSprite_->Draw();

	// 描画後処理
	Sprite::PostDraw();
}

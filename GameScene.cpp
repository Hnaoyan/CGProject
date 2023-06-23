#include "GameScene.h"

void GameScene::Initialize() {
	dxCommon_ = DirectXCommon::GetInstance();
	sprite_ = Sprite::Create();
}

void GameScene::Update() {

}

void GameScene::Draw() {

	// コマンドリストを取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

	Sprite::PreDraw(commandList);

	sprite_->Draw();

	Sprite::PostDraw();

}

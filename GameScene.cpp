#include "GameScene.h"

void GameScene::Initialize() {
	dxCommon_ = DirectXCommon::GetInstance();
	sprite_ = Sprite::Create();
}

void GameScene::Update() {
	// 回転
	transform.rotate.y += 0.01f;
	// WVPMatrixを作成
	Matrix4x4 worldMatrix = MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);
	Matrix4x4 cameraMatrix = MakeAffineMatrix(cameraTransform.scale, cameraTransform.rotate, cameraTransform.translate);
	Matrix4x4 viewMatrix = MakeInverse(cameraMatrix);
	Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(WinApp::kClientWidth) / float(WinApp::kClientHeight), 0.1f, 100.0f);
	Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));
	sprite_->SetWvpMatrix(worldViewProjectionMatrix);

}

void GameScene::Draw() {

	// コマンドリストを取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

	Sprite::PreDraw(commandList);

	sprite_->Draw();

	Sprite::PostDraw();

}

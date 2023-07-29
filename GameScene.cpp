#include "GameScene.h"
#include "ImGuiManager.h"
#include "externals/imgui/imgui_impl_dx12.h"
#include "externals/imgui/imgui_impl_win32.h"


void GameScene::Initialize() {
	dxCommon_ = DirectXCommon::GetInstance();
	sprite_ = Sprite::Create();
}

void GameScene::Update() {
	ImGui::Begin("Setting");
	if (ImGui::TreeNode("Triangle"))
	{
		ImGui::DragFloat3("translate", &triangle1.translate.x, 0.01f, -5.0f, 5.0f);
		ImGui::DragFloat3("scale", &triangle1.scale.x, 0.01f, -5.0f, 5.0f);
		ImGui::DragFloat3("rotate", &triangle1.rotate.x, 0.01f, -5.0f, 5.0f);
		ImGui::Text("%d", IsTriangel);

		ImGui::Checkbox("isEnable", &IsTriangel);
		sprite_->SetIsDepth(IsTriangel);

		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Sprite")) 
	{

		ImGui::TreePop();
	}

	ImGui::End();
	

	// 回転
	//transform.rotate.y += 0.01f;
	// WVPMatrixを作成
	Matrix4x4 worldMatrix = MakeAffineMatrix(triangle1.scale, triangle1.rotate, triangle1.translate);
	Matrix4x4 cameraMatrix = MakeAffineMatrix(cameraTransform.scale, cameraTransform.rotate, cameraTransform.translate);
	Matrix4x4 viewMatrix = MakeInverse(cameraMatrix);
	Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(WinApp::kClientWidth) / float(WinApp::kClientHeight), 0.1f, 100.0f);
	Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));
	sprite_->SetWvpMatrix(worldViewProjectionMatrix);


	Matrix4x4 worldSpriteMatrix = MakeAffineMatrix(spriteTransform.scale,spriteTransform.rotate,spriteTransform.translate);
	Matrix4x4 viewMatrixSprite = MakeIdentity4x4();
	Matrix4x4 projectionMatrixSprite = MakeOrthographicMatrix(0.0f, 0.0f, float(WinApp::kClientWidth), float(WinApp::kClientHeight), 0.0f, 100.0f);
	Matrix4x4 worldViewProjectionMatrixSprite= Multiply(worldSpriteMatrix, Multiply(viewMatrixSprite, projectionMatrixSprite));
	sprite_->SetWvpSpriteMatrix(worldViewProjectionMatrixSprite);
}

void GameScene::Draw() {

	// コマンドリストを取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();
	// 描画前
	Sprite::PreDraw(commandList);

	sprite_->Draw();
	// 描画後
	Sprite::PostDraw();

}

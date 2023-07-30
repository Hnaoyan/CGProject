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
	ImGui::BeginTabBar("Color", 0);
	ImVec4 color = ImVec4(color_.x, color_.y, color_.z, color_.w); // 初期色は赤 (RGBA)
	ImGui::ColorEdit4("Color", (float*)&color); // RGBA色の編集ウィジェット
	color_ = { color.x,color.y,color.z,color.w };
	sprite_->SetColor(color_);
	if (ImGui::TreeNode("Triangle"))
	{
		ImGui::DragFloat3("translate", &triangle1.translate.x, 0.01f, -5.0f, 5.0f);
		ImGui::DragFloat3("scale", &triangle1.scale.x, 0.01f, -5.0f, 5.0f);
		ImGui::DragFloat3("rotate", &triangle1.rotate.x, 0.01f, -5.0f, 5.0f);
		ImGui::Text("%d", IsTriangel);

		ImGui::Checkbox("isEnable", &IsTriangel);
		sprite_->SetIsTriangle(IsTriangel);

		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Sprite")) 
	{
		ImGui::DragFloat3("translateSprite", &spriteTransform.translate.x, 1.0f, 0.0f, 1000.0f);
		ImGui::DragFloat3("scaleSprite", &spriteTransform.scale.x, 0.01f, -5.0f, 5.0f);
		ImGui::DragFloat3("rotateSprite", &spriteTransform.rotate.x, 0.01f, -5.0f, 5.0f);
		ImGui::Text("%d", IsSprite);

		ImGui::Checkbox("isEnableSprite", &IsSprite);
		sprite_->SetIsSprite(IsSprite);

		ImGui::TreePop();
	}
	if (ImGui::TreeNode("Sphere"))
	{
		ImGui::DragFloat3("translateSphere", &SphereTransform.translate.x, 0.01f, -5.0f, 5.0f);
		ImGui::DragFloat3("scaleSphere", &SphereTransform.scale.x, 0.01f, -5.0f, 5.0f);
		ImGui::DragFloat3("rotateSphere", &SphereTransform.rotate.x, 0.01f, -5.0f, 5.0f);
		ImGui::Text("%d", IsSphere);

		ImGui::Checkbox("isEnableSphere", &IsSphere);
		sprite_->SetIsSphere(IsSphere);

		ImGui::TreePop();
	}


	//if (ImGui::TreeNode("OnTop"))
	//{
	//	// ドロップダウンメニューを作成
	//	const char* items[] = { "0", "1"/*, "2", "3", "4", "5"*/ };
	//	ImGui::Combo("Select Value", &onTop, items, IM_ARRAYSIZE(items));
	//	sprite_->SetPattern(onTop);
	//	ImGui::TreePop();

	//}
	ImGui::EndTabBar();
	ImGui::End();
	
	// WVPMatrixを作成
	// 三角の行列
	Matrix4x4 worldMatrix = MakeAffineMatrix(triangle1.scale, triangle1.rotate, triangle1.translate);
	// 球の行列
	Matrix4x4 sphereMatrix = MakeAffineMatrix(SphereTransform.scale, SphereTransform.rotate, SphereTransform.translate);
	// スプライトの行列
	Matrix4x4 worldSpriteMatrix = MakeAffineMatrix(spriteTransform.scale, spriteTransform.rotate, spriteTransform.translate);
	// カメラから描画まで
	Matrix4x4 cameraMatrix = MakeAffineMatrix(cameraTransform.scale, cameraTransform.rotate, cameraTransform.translate);
	Matrix4x4 viewMatrix = MakeInverse(cameraMatrix);
	Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(WinApp::kClientWidth) / float(WinApp::kClientHeight), 0.1f, 100.0f);
	Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));
	// セッター
	sprite_->SetWvpMatrix(worldViewProjectionMatrix);

	// カメラから描画まで
	cameraMatrix = MakeAffineMatrix(cameraSpTransform.scale, cameraSpTransform.rotate, cameraSpTransform.translate);
	viewMatrix = MakeInverse(cameraMatrix);
	projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(WinApp::kClientWidth) / float(WinApp::kClientHeight), 0.1f, 100.0f);
	Matrix4x4 worldSphereViewProjectionMatrix = Multiply(sphereMatrix, Multiply(viewMatrix, projectionMatrix));
	// セッター
	sprite_->SetWvpSphereMatrix(worldSphereViewProjectionMatrix);

	Matrix4x4 viewMatrixSprite = MakeIdentity4x4();
	Matrix4x4 projectionMatrixSprite = MakeOrthographicMatrix(0.0f, 0.0f, float(WinApp::kClientWidth), float(WinApp::kClientHeight), 0.0f, 100.0f);
	Matrix4x4 worldViewProjectionMatrixSprite= Multiply(worldSpriteMatrix, Multiply(viewMatrixSprite, projectionMatrixSprite));
	// セッター
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

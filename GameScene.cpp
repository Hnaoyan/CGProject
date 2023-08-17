#include "GameScene.h"
#include "ImGuiManager.h"
#include "externals/imgui/imgui_impl_dx12.h"
#include "externals/imgui/imgui_impl_win32.h"
#include "Camera.h"
#include "Model.h"


void GameScene::Initialize() {
	dxCommon_ = DirectXCommon::GetInstance();
	sprite_ = Sprite::Create();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	sound_ = audio_->LoadWave("fanfare.wav");

	lighting_ = { { 1.0f,1.0f,1.0f,1.0f } ,{ 0.0f,-1.0f,0.0f }, 1.0f };
}

void GameScene::Update() {
	if (input_->TrigerKey(DIK_SPACE)) {
		audio_->PlayWave(sound_, 0, 0.45f);
	}

	ImGui::Begin("Setting");
	ImGui::DragFloat3("cameraTranslate", &cameraTransform.translate.x, 0.01f, -10.0f, 10.0f);
	ImGui::DragFloat3("cameraScale", &cameraTransform.scale.x, 0.01f, -10.0f, 10.0f);
	ImGui::DragFloat3("cameraRotate", &cameraTransform.rotate.x, 0.01f, -10.0f, 10.0f);

	ImVec4 color = ImVec4(color_.x, color_.y, color_.z, color_.w); // 初期色は赤 (RGBA)
	ImGui::ColorEdit4("Color", (float*)&color); // RGBA色の編集ウィジェット
	color_ = { color.x,color.y,color.z,color.w };
	sprite_->SetColor(color_);

	ImVec4 spColor = ImVec4(sprite_->GetSpriteColor().x, sprite_->GetSpriteColor().y, sprite_->GetSpriteColor().z,
		sprite_->GetSpriteColor().w);
	ImGui::ColorEdit4("SpriteColor", (float*)&spColor);
	sprite_->SetSpriteColor(Vector4(spColor.x, spColor.y, spColor.z, spColor.w));

	if (ImGui::TreeNode("Sprite"))
	{
		ImGui::DragFloat3("translateSprite", &spriteTransform.translate.x, 1.0f, 0.0f, 1000.0f);
		ImGui::DragFloat3("scaleSprite", &spriteTransform.scale.x, 0.01f, -5.0f, 5.0f);
		ImGui::DragFloat3("rotateSprite", &spriteTransform.rotate.x, 0.01f, -5.0f, 5.0f);

		ImGui::Checkbox("isEnableSprite", &IsSprite);
		sprite_->SetIsSprite(IsSprite);

		if (ImGui::TreeNode("uvTransform")) {
			ImGui::DragFloat2("UVTranslate", &spriteUvTransform.translate.x, 0.01f, -10.0f, 10.0f);
			ImGui::DragFloat2("UVScale", &spriteUvTransform.scale.x, 0.01f, -10.0f, 10.0f);
			ImGui::SliderAngle("UVRotate", &spriteUvTransform.rotate.z);
			ImGui::TreePop();
		}

		ImGui::TreePop();
	}
	if (ImGui::TreeNode("Triangle"))
	{
		ImGui::DragFloat3("translate", &triangle1.translate.x, 0.01f, -10.0f, 10.0f);
		ImGui::DragFloat3("scale", &triangle1.scale.x, 0.01f, -5.0f, 5.0f);
		ImGui::DragFloat3("rotate", &triangle1.rotate.x, 0.01f, -5.0f, 5.0f);

		ImGui::Checkbox("isEnable", &IsTriangel);
		sprite_->SetIsTriangle(IsTriangel);

		ImGui::TreePop();
	}
	if (ImGui::TreeNode("Sphere"))
	{
		ImGui::DragFloat3("translateSphere", &SphereTransform.translate.x, 0.01f, -10.0f, 10.0f);
		ImGui::DragFloat3("scaleSphere", &SphereTransform.scale.x, 0.01f, -5.0f, 5.0f);
		ImGui::DragFloat3("rotateSphere", &SphereTransform.rotate.x, 0.01f, -5.0f, 5.0f);

		ImGui::Checkbox("isEnableSphere", &IsSphere);
		sprite_->SetIsSphere(IsSphere);

		if (ImGui::TreeNode("Lighting")) {
			lighting_ = sprite_->GetLighting();
			ImGui::SliderFloat4("color", &lighting_.color.x, 0.0f, 1.0f);
			ImGui::SliderFloat3("direction", &lighting_.direction.x, -1.0f, 1.0f);
			ImGui::DragFloat("intencity", &lighting_.intensity, 0.01f, 0.0f, 10.0f);
			sprite_->SetLighting(lighting_);
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("LightingType"))
		{
			// ドロップダウンメニューを作成
			const char* items[] = { "None", "Lambertian Reflectance", "Half Lambert" };
			lightPattern = sprite_->GetLightPattern();
			ImGui::Combo("Select Value", &lightPattern, items, IM_ARRAYSIZE(items));
			sprite_->SetLightPattern(lightPattern);
			ImGui::TreePop();
		}

		ImGui::TreePop();
	}
	ImGui::End();
	
	// WVPMatrixを作成
	// 三角の行列
	Matrix4x4 worldMatrix = MakeAffineMatrix(triangle1.scale, triangle1.rotate, triangle1.translate);
	Matrix4x4 worldViewProjectionMatrix = Camera::SetViewProjectionMat(triangle1, cameraTransform, 0.45f, 0.1f, 100.0f);
	// セッター
	sprite_->SetWorldMat(worldMatrix);
	sprite_->SetWvpMatrix(worldViewProjectionMatrix);

	// 球の行列
	Matrix4x4 sphereMatrix = MakeAffineMatrix(SphereTransform.scale, SphereTransform.rotate, SphereTransform.translate);
	Matrix4x4 worldSphereViewProjectionMatrix = Camera::SetViewProjectionMat(SphereTransform, cameraTransform, 0.45f, 0.1f, 100.0f);
	// セッター
	sprite_->SetWorldSphereMat(sphereMatrix);
	sprite_->SetWvpSphereMatrix(worldSphereViewProjectionMatrix);

	// スプライトの行列
	Matrix4x4 worldViewProjectionMatrixSprite = Camera::SetViewProjectionMat(spriteTransform, 0, 100.0f);
	// セッター
	sprite_->SetWvpSpriteMatrix(worldViewProjectionMatrixSprite);
	// UV座標系
	Matrix4x4 uvTransformMatrix = MakeScaleMatrix(spriteUvTransform.scale);
	uvTransformMatrix = Multiply(uvTransformMatrix, MakeRotateZMatrix(spriteUvTransform.rotate.z));
	uvTransformMatrix = Multiply(uvTransformMatrix, MakeTranslateMatrix(spriteUvTransform.translate));
	sprite_->SetUVTransformSprite(uvTransformMatrix);


	//if()

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


	Model::PostDraw();

}

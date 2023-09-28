#include "BaseCamera.h"
#include "ImGuiManager.h"

void BaseCamera::Initialize()
{
	// ビューの初期化
	viewProjection_.Initialize();

	// 位置・角度の設定
	viewProjection_.translate_ = { 0,1.5f,-30.0f };
	viewProjection_.rotation_ = { 0,0,0 };
}

void BaseCamera::Update()
{
	ImGui::Begin("base");
	//ImGui::DragFloat3("translate", &viewProjection_.translate_.x, 0.01f, -20.0f, 20.0f);
	// 位置
	ImGui::SliderFloat3("translate", &viewProjection_.translate_.x, -50.0f, 50.0f);
	// 回転
	ImGui::DragFloat3("rotate", &viewProjection_.rotation_.x, 0.01f, -2.0f, 2.0f);
	// 視野角
	ImGui::SliderFloat("fov", &fov_, 5.0f, 100.0f);
	ImGui::End();

	// データの更新と転送
	viewProjection_.SetFov(fov_);
	viewProjection_.UpdateMatrix();
}

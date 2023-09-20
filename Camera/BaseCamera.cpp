#include "BaseCamera.h"
#include "ImGuiManager.h"

void BaseCamera::Initialize()
{
	viewProjection_.Initialize();

	viewProjection_.translate_ = { 0,1.5f,-30.0f };
	viewProjection_.rotation_ = { 0,0,0 };
}

void BaseCamera::Update()
{
	ImGui::Begin("base");
	ImGui::DragFloat3("translate", &viewProjection_.translate_.x, 0.01f, -20.0f, 20.0f);
	ImGui::DragFloat3("rotate", &viewProjection_.rotation_.x, 0.01f, -2.0f, 2.0f);
	ImGui::End();

	// データの更新と転送
	viewProjection_.UpdateMatrix();
}

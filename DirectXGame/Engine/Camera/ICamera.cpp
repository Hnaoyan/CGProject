#include "ICamera.h"
#include "imgui.h"

void ICamera::Initialize()
{
	// ビューの初期化
	viewProjection_.Initialize();

}

void ICamera::Update()
{

//#ifdef _DEBUG
//	// ウィジェットの表示
//	ImGuiWidget();
//
//#endif // _DEBUG


	viewProjection_.SetFov(fov_);
	viewProjection_.UpdateMatrix();
}

void ICamera::ImGuiWidget()
{
	//ImGui::Begin("base");
	//// 位置
	//ImGui::SliderFloat3("translate", &viewProjection_.translate_.x, -50.0f, 50.0f);
	//// 回転
	//ImGui::DragFloat3("rotate", &viewProjection_.rotation_.x, 0.01f, -2.0f, 2.0f);
	//// 視野角
	//ImGui::SliderFloat("fov", &fov_, 5.0f, 100.0f);
	//ImGui::End();
}

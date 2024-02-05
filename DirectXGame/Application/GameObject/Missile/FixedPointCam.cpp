#include "FixedPointCam.h"
#include "imgui.h"
#include "Input.h"
#include "MathCalc.h"

void FixedPointCam::Initialize()
{

	// ビュープロジェクションの初期化を担当
	ICamera::Initialize();

	offset_ = { 0,11.0f,-50.0f };

	viewProjection_.translate_ = offset_;
	viewProjection_.rotation_.x = 0.12f;

}

void FixedPointCam::Update()
{
#ifdef _DEBUG
	ImGuiWidget();
#endif // _DEBUG

	// カメラの回転処理
	//RotationUpdate();

	if (Input::GetInstance()->PressKey(DIK_U)) {
		viewProjection_.rotation_.y += 0.001f;
	}
	else if (Input::GetInstance()->PressKey(DIK_I)) {
		viewProjection_.rotation_.y -= 0.001f;
	}

	// 基本情報のウィジェットと行列の更新
	ICamera::Update();
}

void FixedPointCam::ImGuiWidget()
{

	ImGui::Begin("FixedPointCam");
	// 位置
	ImGui::SliderFloat3("translate", &viewProjection_.translate_.x, -50.0f, 50.0f);
	// 回転
	ImGui::DragFloat3("rotate", &viewProjection_.rotation_.x,0.01f,0,3.14f);
	// 視野角
	ImGui::SliderFloat("fov", &fov_, 5.0f, 100.0f);

	ImGui::DragFloat3("offSet", &offset_.x, 0.01f, 0, 100.0f);

	ImGui::End();

}

void FixedPointCam::RotationUpdate()
{
	XINPUT_STATE joyState;
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		float rotateSpeed = 0.02f;

		viewProjection_.rotation_.y +=
			(float)joyState.Gamepad.sThumbLX / SHRT_MAX * rotateSpeed;
	}
	// 回転行列の合成
	Matrix4x4 rotateMatrix = MatLib::Multiply(
		MatLib::MakeRotateXMatrix(viewProjection_.rotation_.x),
		MatLib::Multiply(
			MatLib::MakeRotateYMatrix(viewProjection_.rotation_.y),
			MatLib::MakeRotateZMatrix(viewProjection_.rotation_.z)));

	viewProjection_.translate_ = MatLib::Transform(offset_, rotateMatrix);
	viewProjection_.UpdateMatrix();
}

#include "FollowCamera.h"
#include "MathCalc.h"
#include <Input.h>
#include <ImGuiManager.h>

void FollowCamera::Initialize() {
	// ビュープロジェクションの初期化
	viewProjection_.Initialize();
	defaultTranslate_ = viewProjection_.translate_;
	defaultOffset = {0.0f, 2.0f, -15.0f};
}

void FollowCamera::Update() {

	XINPUT_STATE joyState;

	// 追従対象がいれば
	if (target_) {
		// 座標をコピーしてオフセット分ずらす
		Vector3 worldPosition = { target_->matWorld_.m[3][0],target_->matWorld_.m[3][1],target_->matWorld_.m[3][2] };

		if (Input::GetInstance()->GetJoystickState(0, joyState)) {
			float rotateSpeed = 0.02f;

			viewProjection_.rotation_.y += (float)joyState.Gamepad.sThumbRX / SHRT_MAX * rotateSpeed;
			viewProjection_.rotation_.x += (float)joyState.Gamepad.sThumbRY / SHRT_MAX * rotateSpeed;

		}

		interTarget_ = MathCalc::Lerp(interTarget_, worldPosition, 0.1f);
		Vector3 offset = SetOffset();
		viewProjection_.translate_ = VectorLib::Add(interTarget_, offset);

		ImGui::Begin("followCamera");
		ImGui::DragFloat3("interT", &interTarget_.x, 0.01f, -100, 100);
		ImGui::DragFloat3("translate", &worldPosition.x, 0.01f, -100, 100);
		ImGui::End();

	}

	// ビュー行列の更新・転送
	viewProjection_.UpdateMatrix();
}

void FollowCamera::Reset()
{
	if (target_) {
		// 追従対象・角度初期化
		//viewProjection_.rotation_.y = 0;
		//Vector3 worldPosition = { target_->matWorld_.m[3][0],target_->matWorld_.m[3][1],target_->matWorld_.m[3][2] };
		interTarget_ = target_->translation_;
		viewProjection_.rotation_.y = target_->rotation_.y;
	}

	destinationAngleY_ = viewProjection_.rotation_.y;
	//destinationAngleY_ = 0;
	// 追従対象からのオフセット
	Vector3 offset = SetOffset();
	viewProjection_.translate_ = VectorLib::Add(interTarget_, offset);
}

Vector3 FollowCamera::SetOffset() const
{
	// カメラまでのオフセット
	Vector3 offset = defaultOffset;
	// 回転行列の合成
	Matrix4x4 rotateMatrix = MatLib::Multiply(
		MatLib::MakeRotateXMatrix(viewProjection_.rotation_.x),
		MatLib::Multiply(
			MatLib::MakeRotateYMatrix(viewProjection_.rotation_.y),
			MatLib::MakeRotateZMatrix(viewProjection_.rotation_.z)));
	offset = MatLib::Transform(offset, rotateMatrix);
	return offset;
}

void FollowCamera::SetTarget(const WorldTransform* target) {
	target_ = target;
	Reset();
}
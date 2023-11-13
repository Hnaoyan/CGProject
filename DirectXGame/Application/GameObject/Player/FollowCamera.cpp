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
		interTarget_ = MathCalc::Lerp(interTarget_, worldPosition, 0.0001f);

		if (Input::GetInstance()->GetJoystickState(0, joyState)) {
			float rotateSpeed = 0.02f;

			destinationAngleY_ += (float)joyState.Gamepad.sThumbRX / SHRT_MAX * rotateSpeed;
			//viewProjection_.rotation_.y += (float)joyState.Gamepad.sThumbRX / SHRT_MAX * rotateSpeed;
			viewProjection_.rotation_.x += (float)joyState.Gamepad.sThumbRY / SHRT_MAX * rotateSpeed;

			// スティックの入力がなくなったらリセット
			if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {
				Reset();
			}
		}
		
		viewProjection_.rotation_.y =
			MathCalc::LerpShortAngle(viewProjection_.rotation_.y, destinationAngleY_, 1.0f / 30.0f);

		if (isReset_) {
			reset_t_ += 0.1f;
			if (reset_t_ <= 1.0f) {
				viewProjection_.rotation_.y =
					MathCalc::EaseInCubicF(viewProjection_.rotation_.y, target_->rotation_.y, reset_t_);
				reset_t_ = 0;
				isReset_ = false;
			}
		}

		viewProjection_.translate_ = VectorLib::Add(worldPosition, SetOffset());

		ImGui::Begin("camera");
		ImGui::DragFloat3("pos", &viewProjection_.translate_.x, 0.01f, -100.0f, 100.0f);
		ImGui::Text("%f : %f : %f", target_->translation_.x, target_->translation_.y, target_->translation_.z);
		ImGui::Text("rate_t : %f", defaultRate_t_);
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
		Vector3 worldPosition = { target_->matWorld_.m[3][0],target_->matWorld_.m[3][1],target_->matWorld_.m[3][2] };
		interTarget_ = worldPosition;
		viewProjection_.rotation_.y = target_->rotation_.y;
	}

	destinationAngleY_ = viewProjection_.rotation_.y;
	//destinationAngleY_ = 0;
	// 追従対象からのオフセット
	Vector3 offset = SetOffset();
	viewProjection_.translate_ = VectorLib::Add(interTarget_, offset);
	defaultRate_t_ = 0;
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
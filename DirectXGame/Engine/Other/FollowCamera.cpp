#include "FollowCamera.h"
#include "MathCalc.h"
#include <Input.h>

void FollowCamera::Initialize() {
	// ビュープロジェクションの初期化
	viewProjection_.Initialize();
	defaultTranslate_ = viewProjection_.translate_;
	offset = {0.0f, 2.0f, -12.5f};
}

void FollowCamera::Update() {

	XINPUT_STATE joyState;

	// 追従対象がいれば
	if (target_) {
		if (Input::GetInstance()->GetJoystickState(0, joyState)) {
			interTarget_ = MathCalc::Lerp(interTarget_, target_->translation_, 0.01f);
		}
		if (Input::GetInstance()->GetJoystickState(0, joyState)) {
			float rotateSpeed = 0.02f;

			//destinationAngleY_ += (float)joyState.Gamepad.sThumbRX / SHRT_MAX * rotateSpeed;
			viewProjection_.rotation_.y += (float)joyState.Gamepad.sThumbRX / SHRT_MAX * rotateSpeed;
			
			// スティックの入力がなくなったらリセット
			if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {
				Reset();
			}
		}
		//viewProjection_.rotation_.y =
		//	MathCalc::LerpShortAngle(viewProjection_.rotation_.y, destinationAngleY_, 0.5f);

		if (isReset_) {
			reset_t_ += 0.1f;
			if (reset_t_ <= 1.0f) {
				viewProjection_.rotation_.y =
					MathCalc::Lerp(viewProjection_.rotation_.y, target_->rotation_.y, reset_t_);
				reset_t_ = 0;
				isReset_ = false;
			}
		}

	}

	// 座標をコピーしてオフセット文ずらす
	viewProjection_.translate_ = VectorLib::Add(target_->translation_, SetOffset());

	// ビュー行列の更新・転送
	viewProjection_.UpdateMatrix();
}

void FollowCamera::Reset()
{
	if (target_) {
		// 追従対象・角度初期化
		//interTarget_ = target_->translation_;
		viewProjection_.rotation_.y = 0;
	}
}

Vector3 FollowCamera::SetOffset() const
{
	// カメラまでのオフセット
	Vector3 Offset = defaultOffset;
	// 回転行列の合成
	Matrix4x4 rotateMatrix = MatLib::Multiply(
		MatLib::MakeRotateXMatrix(viewProjection_.rotation_.x),
		MatLib::Multiply(
			MatLib::MakeRotateYMatrix(viewProjection_.rotation_.y),
			MatLib::MakeRotateZMatrix(viewProjection_.rotation_.z)));
	Offset = MatLib::Transform(Offset, rotateMatrix);
	return Offset;
}
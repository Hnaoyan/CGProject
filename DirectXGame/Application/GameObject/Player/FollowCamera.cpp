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
			interTarget_ = MathCalc::Lerp(interTarget_, worldPosition, 0.01f);
		}
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

		//if (target_) {
		//	interTarget_ = MathCalc::Lerp(interTarget_, worldPosition, defaultRate_t_);
		//}

		//Vector3 off = SetOffset();
		//viewProjection_.translate_ = VectorLib::Add(interTarget_, SetOffset());

		viewProjection_.translate_ = VectorLib::Add(worldPosition, SetOffset());

		ImGui::Begin("camera");
		ImGui::DragFloat3("pos", &viewProjection_.translate_.x, 0.01f, -100.0f, 100.0f);
		ImGui::Text("%f : %f : %f", target_->translation_.x, target_->translation_.y, target_->translation_.z);
		ImGui::Text("rate_t : %f", defaultRate_t_);
		ImGui::End();

	}
	// ビュー行列の更新・転送
	viewProjection_.UpdateMatrix();

	//Vector3 from0 = MathCalc::Normalize(Vector3{ 1.0f,0.7f,0.5f });
	//Vector3 to0 = VectorLib::Scaler(from0, -1.0f);
	//Vector3 from1 = MathCalc::Normalize(Vector3{ -0.6f,0.9f,0.2f });
	//Vector3 to1 = MathCalc::Normalize(Vector3{ 0.4f,0.7f,-0.5f });

	//Matrix4x4 rot = MatLib::DirectionToDirection(MathCalc::Normalize(Vector3{ 1.0f,0,0 }), MathCalc::Normalize(Vector3{ -1.0f,0,0 }));

	//ImGui::Begin("Mat");
	//ImGui::Text("%.2f : %.2f : %.2f : %.2f", rot.m[0][0], rot.m[0][1], rot.m[0][2], rot.m[0][3]);
	//ImGui::Text("%.2f : %.2f : %.2f : %.2f", rot.m[1][0], rot.m[1][1], rot.m[1][2], rot.m[1][3]);
	//ImGui::Text("%.2f : %.2f : %.2f : %.2f", rot.m[2][0], rot.m[2][1], rot.m[2][2], rot.m[2][3]);
	//ImGui::Text("%.2f : %.2f : %.2f : %.2f", rot.m[3][0], rot.m[3][1], rot.m[3][2], rot.m[3][3]);
	//ImGui::End();
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
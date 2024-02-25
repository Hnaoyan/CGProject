#include "FollowCamera.h"
#include "MathCalc.h"
#include "GlobalVariables.h"

#include <Input.h>
#include <ImGuiManager.h>

void FollowCamera::Initialize() {
	// ビュープロジェクションの初期化
	viewProjection_.Initialize();
	defaultTranslate_ = viewProjection_.translate_;
	defaultOffset = {0.0f, 6.0f, -35.0f};

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "FollowCamera";
	// グループを追加
	GlobalVariables::GetInstance()->CreateGroup(groupName);
	globalVariables->AddItem(groupName, "delayRate", delayRate_);

	ApplyGlobalVariables();
}

void FollowCamera::Update() {
	// jsonの更新
	ApplyGlobalVariables();

	XINPUT_STATE joyState;

	// 追従対象がいれば
	if (target_) {
		// 座標をコピーしてオフセット分ずらす
		Vector3 worldPosition = { target_->matWorld_.m[3][0],target_->matWorld_.m[3][1],target_->matWorld_.m[3][2] };

		if (Input::GetInstance()->GetJoystickState(0, joyState)) {
			float rotateSpeed = 0.02f;

			destinationAngleY_ += (float)joyState.Gamepad.sThumbRX / SHRT_MAX * rotateSpeed;
			viewProjection_.rotation_.x += (float)joyState.Gamepad.sThumbRY / SHRT_MAX * rotateSpeed;

		}

		//viewProjection_.rotation_.y = MathCalc::LerpShortAngle(viewProjection_.rotation_.y, destinationAngleY_, 0.1f);

		// 遅延追尾
		interTarget_ = MathCalc::Lerp(interTarget_, worldPosition, delayRate_);
		Vector3 offset = SetOffset();
		viewProjection_.translate_ = VectorLib::Add(interTarget_, offset);
	}

	// ビュー行列の更新・転送
	Vector3 move = targetObject_->GetMoveVector();
	viewProjection_.rotation_.y = MathCalc::CalculateYawFromVector(Vector3(move.x, 0, move.z));
	viewProjection_.UpdateMatrix();
}

void FollowCamera::Reset()
{
	if (target_) {
		// 追従対象・角度初期化
		Vector3 worldPosition = { target_->matWorld_.m[3][0],target_->matWorld_.m[3][1],target_->matWorld_.m[3][2] };
		interTarget_ = worldPosition;
		viewProjection_.rotation_.y = target_->rotation_.y;
	}

	destinationAngleY_ = viewProjection_.rotation_.y;
	// 追従対象からのオフセット
	Vector3 offset = SetOffset();
	viewProjection_.translate_ = VectorLib::Add(interTarget_, offset);
}

void FollowCamera::ApplyGlobalVariables()
{
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "FollowCamera";
	// 移動量
	delayRate_ = globalVariables->GetFloatValue(groupName, "delayRate");
}

Vector3 FollowCamera::SetOffset() const
{
	// カメラまでのオフセット
	Vector3 offset = defaultOffset;
	//if (targetObject_) {
	//	float distance = 50.0f;
	//	offset = targetObject_->GetMoveVector() * -distance;
	//}
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
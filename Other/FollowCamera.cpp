#include "FollowCamera.h"
#include "Vector3Math.h"
#include "MatrixMath.h"
#include <Input.h>

void FollowCamera::Initialize() {
	// ビュープロジェクションの初期化
	viewProjection_.Initialize();

	offset = {0.0f, 2.0f, -12.5f};
}

void FollowCamera::Update() {

	XINPUT_STATE joyState;

	// 追従対象がいれば
	if (target_) {
		if (Input::GetInstance()->GetJoystickState(0, joyState)) {
			
		}

		switch (cameraState_) {
		case FollowCamera::CameraView::kNormal:
			offset = {0.0f, 2.0f, -12.5f};
			break;
		case FollowCamera::CameraView::kShoot:
			offset = {5.0f, 2.0f, -12.5f};
			break;
		}
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_X && !isChange) {
			switch (cameraState_) {
			case FollowCamera::CameraView::kNormal:
				cameraState_ = CameraView::kShoot;
				isChange = true;
				break;
			case FollowCamera::CameraView::kShoot:
				cameraState_ = CameraView::kNormal;
				isChange = true;
				break;
			}
		}

		if (isChange) {
			changeTime++;
			if (changeTime == 60) {
				isChange = false;
				changeTime = 0;
			}
		}

		Matrix4x4 rotateMatrix = MatrixMath::Multiply(
		    MatrixMath::MakeRotateXMatrix(viewProjection_.rotation_.x),
		    MatrixMath::Multiply(
		        MatrixMath::MakeRotateYMatrix(viewProjection_.rotation_.y),
		        MatrixMath::MakeRotateZMatrix(viewProjection_.rotation_.z)));
		offset = MatrixMath::Transform(offset, rotateMatrix);

		// 座標をコピーしてオフセット文ずらす
		viewProjection_.translate_ = Vector3Math::Add(target_->translation_, offset);
	}

	// ビュー行列の更新・転送
	viewProjection_.UpdateMatrix();
	//viewProjection_.TransferMatrix();
}
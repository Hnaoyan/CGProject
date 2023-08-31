#include "TitleCamera.h"
#include "Vector3Math.h"
#include "MatrixMath.h"
#include "MatrixMath.h"
#include <Input.h>

void TitleCamera::Initialize() {
	// ビュープロジェクションの初期化
	viewProjection_.Initialize();

	offset = { 0.0f, 2.0f, -12.5f };

	worldtransform_.translation_ = { 0,0,-15.0f };
	worldtransform_.rotation_ = { 0,0,0 };
	worldtransform_.scale_ = { 1.0f,1.0f,1.0f };
}

void TitleCamera::Update() {

	XINPUT_STATE joyState;

	// 追従対象がいれば
	if (target_) {
		if (Input::GetInstance()->GetJoystickState(0, joyState)) {

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

	//worldtransform_.matWorld_ = MatrixMath::MakeAffineMatrix(worldtransform_.scale_, worldtransform_.rotation_, worldtransform_.translation_);
	//viewProjection_.matView = worldtransform_.matWorld_;
	viewProjection_.translate_ = worldtransform_.translation_;
	viewProjection_.scale_ = worldtransform_.scale_;
	viewProjection_.rotation_ = worldtransform_.rotation_;
	// ビュー行列の更新
	viewProjection_.UpdateMatrix();
	viewProjection_.TransferMatrix();
}
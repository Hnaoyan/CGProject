#include "DebugCamera.h"

void DebugCamera::Update()
{
	bool dirty = false;
	Vector2 angle = { 0,0 };

	// マウスの入力を取得
	Input::MouseMove mouseMove = input_->GetMouseMove();

	// マウスの左ボタンなら回転
	if (input_->IsPressMouse(0)) {
		Vector2 d = { mouseMove.lX * scale_.x,mouseMove.lY * scale_.y };

		angle.x = -d.x * float(std::numbers::pi);
		angle.y = -d.y * float(std::numbers::pi);
		dirty = true;
	}

	// マウスの中ボタンなら平行移動
	if (input_->IsPressMouse(2)) {
		Vector2 d = { mouseMove.lX / 100.0f,mouseMove.lY / 100.0f };

		Vector3 move = { -d.x,+d.y,0 };
		move = MatLib::Transform(move, matRot);

		viewProjection_.translate_.x += move.x;
		viewProjection_.translate_.y += move.y;
		viewProjection_.translate_.z += move.z;

		viewProjection_.rotation_.x += move.x;
		viewProjection_.rotation_.y += move.y;
		viewProjection_.rotation_.z += move.z;

		dirty = true;
	}

	if (mouseMove.lZ != 0) {
		distance_ -= mouseMove.lZ / 100.0f;
		distance_ = max(distance_, 1.0f);
		dirty = true;
	}

	if (dirty) {
		// 回転行列の生成
		Matrix4x4 matRotNew = MatLib::MakeIdentity4x4();
		matRotNew = MatLib::Multiply(matRotNew, MatLib::MakeRotateXMatrix(-angle.x));
		matRotNew = MatLib::Multiply(matRotNew, MatLib::MakeRotateYMatrix(-angle.y));

		MultiplyMatrix(matRotNew);
	}

	viewProjection_.UpdateMatrix();

}

void DebugCamera::MultiplyMatrix(const Matrix4x4& matrix)
{
	matRot = MatLib::Multiply(matrix, matRot);

}

#pragma once
#include "Input.h"
#include "ViewProjection.h"
#include "StructManager.h"
#include "MathCalc.h"

class DebugCamera
{
public:
	DebugCamera(int window_width, int window_height);

	void Update();

	void SetDistance(float distance);


private:
	// 入力クラスのポインタ
	Input* input_;
	// カメラ注視点までの距離
	float distance_ = 50.0f;

	// スケール
	Vector2 scale_ = { 1.0f,1.0f };

	// 回転行列
	Matrix4x4 matRot = MatrixMath::MakeIdentity4x4();
	// ビュープロジェクション
	ViewProjection viewProjection_;

	void MultiplyMatrix(const Matrix4x4& matrix);
};


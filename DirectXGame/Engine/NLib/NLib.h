#pragma once
#include "StructManager.h"
#include "MathCalc.h"

namespace NLib
{
	// サーカス用関数
	float InductionPerform(float frame, float accuracy);
	Vector3 InductionNewVector(const Vector3& missile, const Vector3& target, float p);

	// 球面線形補間
	Vector3 Slerp(const Vector3& moveVector, const Vector3& targetVector, float t);
	Vector3 Slerp_Test(const Vector3& moveVector, const Vector3& targetVector, float t);

	// デルタタイム計算
	float DeltaTimeSpeed(float kSpeed);
	float GetDeltaTime(float frame);

};


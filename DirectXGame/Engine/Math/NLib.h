#pragma once
#include "StructManager.h"

class NLib
{
public:
	// サーカス用関数
	static float InductionPerform(float frame, float accuracy);
	static Vector3 InductionNewVector(const Vector3& missile, const Vector3& target, float frame, float accuracy);
	
	// 球面線形補間
	static Vector3 Slerp(const Vector3& moveVector, const Vector3& targetVector, float t);
	
	// デルタタイム計算
	static float DeltaTimeSpeed(float kSpeed);

};


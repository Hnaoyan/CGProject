#pragma once
#include "StructManager.h"
#include "MathCalc.h"

class NLib
{
public:
	//using Normalize =  MathCalc::Normalize
	//template<class T> using Normalize = MathCalc::Normalize<T>;

public:
	// サーカス用関数
	static float InductionPerform(float frame, float accuracy);
	static Vector3 InductionNewVector(const Vector3& missile, const Vector3& target, float p);
	
	// 球面線形補間
	static Vector3 Slerp(const Vector3& moveVector, const Vector3& targetVector, float t);
	static Vector3 Slerp_Test(const Vector3& moveVector, const Vector3& targetVector, float t);

	// デルタタイム計算
	static float DeltaTimeSpeed(float kSpeed);

};


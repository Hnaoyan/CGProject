#pragma once
#include "StructManager.h"
#include "Vector3Math.h"
#include "Vector4Math.h"
#include "MatrixMath.h"
#include "Matrix.h"

class MathCalc 
{
public:
	static Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m);

	// 長さ
	static float Length(const Vector2& v1);
	static float Length(const Vector3& v1);
	// 内積
	static float Dot(const Vector2& v1, const Vector2& v2);
	static float Dot(const Vector3& v1, const Vector3& v2);
	// クロス積
	static Vector3 Cross(const Vector3& v1, const Vector3& v2);

	// 距離
	static float Distance(const Vector2& v1, const Vector2& v2);

	static Vector2 Normalize(const Vector2& v);
	static Vector3 Normalize(const Vector3& v);

	static Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t);
	static Vector3 Slerp(const Vector3& v1, const Vector3& v2, float t);
};

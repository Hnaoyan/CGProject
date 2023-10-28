#pragma once
#include "StructManager.h"
#include "VectorLib.h"
#include "MatLib.h"

class MathCalc
{
private:
	/// <summary>
	/// イージングの結果を出す計算
	/// </summary>
	/// <param name="t"></param>
	/// <param name="start"></param>
	/// <param name="end"></param>
	/// <returns></returns>
	// Vector3
	static Vector3 Ease(float t, Vector3& start, Vector3& end);
	// Vector2
	static Vector2 Ease(float t, Vector2& start, Vector2& end);
	// float
	static float Ease(float t, float start, float end);

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

	// 正規化
	static Vector2 Normalize(const Vector2& v);
	static Vector3 Normalize(const Vector3& v);

	static Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t);
	static Vector3 Slerp(const Vector3& v1, const Vector3& v2, float t);

#pragma region float Easing
	static float EaseInQuadF(float t, float start, float end);

	static float EaseOutQuadF(float t, float start, float end);

	static float EaseInOutQuadF(float t, float start, float end);

	static float EaseInCubicF(float t, float start, float end);

	static float EaseOutCubicF(float t, float start, float end);

	static float EaseInOutCubicF(float t, float start, float end);
#pragma endregion

#pragma region Vector2 Easing
	static Vector2 EaseInQuadF(float t, Vector2 start, Vector2 end);

	static Vector2 EaseOutQuadF(float t, Vector2 start, Vector2 end);

	static Vector2 EaseInOutQuadF(float t, Vector2 start, Vector2 end);

	static Vector2 EaseInCubicF(float t, Vector2 start, Vector2 end);

	static Vector2 EaseOutCubicF(float t, Vector2 start, Vector2 end);

	static Vector2 EaseInOutCubicF(float t, Vector2 start, Vector2 end);
#pragma endregion

#pragma region Vector3 Easing
	static Vector3 EaseInQuadF(float t, Vector3 start, Vector3 end);

	static Vector3 EaseOutQuadF(float t, Vector3 start, Vector3 end);

	static Vector3 EaseInOutQuadF(float t, Vector3 start, Vector3 end);

	static Vector3 EaseInCubicF(float t, Vector3 start, Vector3 end);

	static Vector3 EaseOutCubicF(float t, Vector3 start, Vector3 end);

	static Vector3 EaseInOutCubicF(float t, Vector3 start, Vector3 end);
#pragma endregion
};

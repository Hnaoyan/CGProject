#include "MathCalc.h"
#include <cmath>
#include <algorithm>
#include <numbers>

Vector3 MathCalc::TransformNormal(const Vector3& v, const Matrix4x4& m) {
	Vector3 result{
		v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0],
		v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1],
		v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2],
	};
	return result;
}

float MathCalc::Length(const Vector2& v1) {
	float result;
	result = sqrt(powf(v1.x, 2) + powf(v1.y, 2));
	return result;
}
float MathCalc::Length(const Vector3& v1) {
	float result;
	result = sqrt(powf(v1.x, 2) + powf(v1.y, 2) + powf(v1.z, 2));
	return result;
}

float MathCalc::Dot(const Vector2& v1, const Vector2& v2) {
	float result;
	result = v1.x * v2.x + v1.y * v2.y;
	return result;
}
float MathCalc::Dot(const Vector3& v1, const Vector3& v2) {
	float result;
	result = v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	return result;
}

Vector3 MathCalc::Cross(const Vector3& v1, const Vector3& v2) {
	Vector3 result = {};
	result = {
		(v1.y * v2.z - v1.z * v2.y), (v1.z * v2.x - v1.x * v2.z), (v1.x * v2.y - v1.y * v2.x) };
	return result;
}

float MathCalc::Distance(const Vector2& v1, const Vector2& v2) {
	float result;
	Vector2 distace = { v1.x - v2.x, v1.y - v2.y };
	result = sqrt(powf(distace.x, 2) + powf(distace.y, 2));
	return result;
}

Vector2 MathCalc::Normalize(const Vector2& v) {
	Vector2 result{};
	float length = Length(v);

	if (length != 0) {
		result.x = v.x / length;
		result.y = v.y / length;
	}
	return result;
}

Vector3 MathCalc::Normalize(const Vector3& v) {
	Vector3 result{};
	float length = Length(v);

	if (length != 0) {
		result.x = v.x / length;
		result.y = v.y / length;
		result.z = v.z / length;
	}
	return result;
}

Vector3 MathCalc::Lerp(const Vector3& v1, const Vector3& v2, float t) {
	Vector3 result;

	result.x = (1.0f - t) * v1.x + t * v2.x;
	result.y = (1.0f - t) * v1.y + t * v2.y;

	return result;
}

float MathCalc::Lerp(const float a, const float b, float t)
{
	float t_ = 0;
	t_ += t;
	t_ = std::clamp(t_, 0.0f, 1.0f);
	float result = (1.0f - t) * a + t * b;
	return result;
}

Vector3 MathCalc::Slerp(const Vector3& v1, const Vector3& v2, float t) {
	Vector3 result = {};

	float Outer = v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;

	Outer = Outer * t;

	return result;
}

float MathCalc::LerpShortAngle(float a, float b, float t)
{
	// 角度差分
	float diff = b - a;
	diff = std::fmodf(diff, 2.0f * float(std::numbers::pi));
	// 角度を[-PI, +PI]に補正
	diff = std::fmodf(diff + (float(std::numbers::pi)), 2.0f * (float(std::numbers::pi))) - (float(std::numbers::pi));
	diff = Lerp(a, diff, t);
	return diff;
	// // 角度差分
	//float diff = b - a;

	//// 角度を正規化
	//while (diff >= 360.0f) {
	//	diff -= 360.0f;
	//}
	//while (diff < 0.0f) {
	//	diff += 360.0f;
	//}

	//// 補間率を0から1の範囲に制約
	//t = std::clamp(t, 0.0f, 1.0f);

	//// 角度を補間
	//diff = Lerp(a, a + diff, t);

	//return diff;
}

#pragma region イージング計算
Vector3 MathCalc::Ease(float t, Vector3& start, Vector3& end)
{
	return Vector3((1.0f - t) * start.x + t * end.x,
		(1.0f - t) * start.y + t * end.y,
		(1.0f - t) * start.z + t * end.z);
}

Vector2 MathCalc::Ease(float t, Vector2& start, Vector2& end)
{
	return Vector2((1.0f - t) * start.x + t * end.x,
		(1.0f - t) * start.y + t * end.y);
}

float MathCalc::Ease(float t, float start, float end)
{
	return ((1.0f - t) * start + t * end);
}
#pragma endregion

#pragma region float Easing

float MathCalc::EaseInQuadF(float t, float start, float end) {
	float easeT = t * t;
	return (1.0f - easeT) * start + easeT * end;
}

float MathCalc::EaseOutQuadF(float t, float start, float end) {
	float easeT = 1.0f - powf(1.0f - t, 2.0f);
	return (1.0f - easeT) * start + easeT * end;
}

float MathCalc::EaseInOutQuadF(float t, float start, float end) {
	float easeT = t > 0.5f ? 2.0f * t * t
		: 1.0f - powf(-2.0f * t + 2.0f, 2.0f) / 2.0f;

	return (1.0f - easeT) * start + easeT * end;
}

float MathCalc::EaseInCubicF(float t, float start, float end) {
	float easeT = t * t * t;
	return (1.0f - easeT) * start + easeT * end;
}

float MathCalc::EaseOutCubicF(float t, float start, float end) {
	float easeT = 1.0f - powf(1.0f - t, 3.0f);
	return (1.0f - easeT) * start + easeT * end;
}

float MathCalc::EaseInOutCubicF(float t, float start, float end) {
	float easeT = t > 0.5f ? 4.0f * t * t * t
		: 1.0f - powf(-2.0f * t + 2, 3.0f) / 2.0f;

	return (1.0f - easeT) * start + easeT * end;
}
#pragma endregion

#pragma region Vector2 Easing
Vector2 MathCalc::EaseInQuadF(float t, Vector2 start, Vector2 end) {
	float easeT = t * t;
	return Vector2(
		(1.0f - easeT) * start.x + easeT * end.x,
		(1.0f - easeT) * start.y + easeT * end.y);
}
Vector2 MathCalc::EaseOutQuadF(float t, Vector2 start, Vector2 end) {
	float easeT = 1.0f - powf(1.0f - t, 2.0f);
	return Vector2(
		(1.0f - easeT) * start.x + easeT * end.x,
		(1.0f - easeT) * start.y + easeT * end.y);
}
Vector2 MathCalc::EaseInOutQuadF(float t, Vector2 start, Vector2 end) {
	float easeT = t > 0.5f ? 2.0f * t * t : 1.0f - powf(-2.0f * t + 2.0f, 2.0f) / 2.0f;

	return Vector2(
		(1.0f - easeT) * start.x + easeT * end.x,
		(1.0f - easeT) * start.y + easeT * end.y);
}

Vector2 MathCalc::EaseInCubicF(float t, Vector2 start, Vector2 end) {
	float easeT = t * t * t;
	return Vector2(
		(1.0f - easeT) * start.x + easeT * end.x,
		(1.0f - easeT) * start.y + easeT * end.y);
}
Vector2 MathCalc::EaseOutCubicF(float t, Vector2 start, Vector2 end) {
	float easeT = 1.0f - powf(1.0f - t, 3.0f);
	return Vector2(
		(1.0f - easeT) * start.x + easeT * end.x,
		(1.0f - easeT) * start.y + easeT * end.y);
}
Vector2 MathCalc::EaseInOutCubicF(float t, Vector2 start, Vector2 end) {
	float easeT = t > 0.5f ? 4.0f * t * t * t : 1.0f - powf(-2.0f * t + 2, 3.0f) / 2.0f;

	return Vector2(
		(1.0f - easeT) * start.x + easeT * end.x,
		(1.0f - easeT) * start.y + easeT * end.y);
}


#pragma endregion

#pragma region Vector3 Easing
Vector3 MathCalc::EaseInQuadF(float t, Vector3 start, Vector3 end) {
	float easeT = t * t;
	return Vector3(
		(1.0f - easeT) * start.x + easeT * end.x,
		(1.0f - easeT) * start.y + easeT * end.y,
		(1.0f - easeT) * start.z + easeT * end.z);
}
Vector3 MathCalc::EaseOutQuadF(float t, Vector3 start, Vector3 end) {
	float easeT = 1.0f - powf(1.0f - t, 2.0f);
	return Vector3(
		(1.0f - easeT) * start.x + easeT * end.x,
		(1.0f - easeT) * start.y + easeT * end.y,
		(1.0f - easeT) * start.z + easeT * end.z);
}
Vector3 MathCalc::EaseInOutQuadF(float t, Vector3 start, Vector3 end) {
	float easeT = t > 0.5f ? 2.0f * t * t : 1.0f - powf(-2.0f * t + 2.0f, 2.0f) / 2.0f;

	return Vector3(
		(1.0f - easeT) * start.x + easeT * end.x,
		(1.0f - easeT) * start.y + easeT * end.y,
		(1.0f - easeT) * start.z + easeT * end.z);
}

Vector3 MathCalc::EaseInCubicF(float t, Vector3 start, Vector3 end) {
	float easeT = t * t * t;
	return Vector3(
		(1.0f - easeT) * start.x + easeT * end.x,
		(1.0f - easeT) * start.y + easeT * end.y,
		(1.0f - easeT) * start.z + easeT * end.z);
}
Vector3 MathCalc::EaseOutCubicF(float t, Vector3 start, Vector3 end) {
	float easeT = 1.0f - powf(1.0f - t, 3.0f);
	return Vector3(
		(1.0f - easeT) * start.x + easeT * end.x,
		(1.0f - easeT) * start.y + easeT * end.y,
		(1.0f - easeT) * start.z + easeT * end.z);
}
Vector3 MathCalc::EaseInOutCubicF(float t, Vector3 start, Vector3 end) {
	float easeT = t > 0.5f ? 4.0f * t * t * t : 1.0f - powf(-2.0f * t + 2, 3.0f) / 2.0f;
	return Ease(easeT, start, end);
}
#pragma endregion

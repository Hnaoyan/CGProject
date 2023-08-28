#include "MathCalc.h"
#include <cmath>

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
	    (v1.y * v2.z - v1.z * v2.y), (v1.z * v2.x - v1.x * v2.z), (v1.x * v2.y - v1.y * v2.x)};
	return result;
}

float MathCalc::Distance(const Vector2& v1, const Vector2& v2) {
	float result;
	Vector2 distace = {v1.x - v2.x, v1.y - v2.y};
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

Vector3 MathCalc::Slerp(const Vector3& v1, const Vector3& v2, float t) {
	Vector3 result = {};

	float Outer = v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;

	Outer = Outer * t;

	return result;
}
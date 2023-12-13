#include "VectorLib.h"

#pragma region Vector4系
Vector4 VectorLib::Add(const Vector4& v1, const Vector4& v2)
{
	Vector4 result{};

	result.x = v1.x + v2.x;
	result.y = v1.y + v2.y;
	result.z = v1.z + v2.z;
	result.w = v1.w + v2.w;

	return result;
}
#pragma endregion

#pragma region Vector3系
Vector3 VectorLib::Add(const Vector3& v1, const Vector3& v2) {
	Vector3 v{};

	v.x = v1.x + v2.x;
	v.y = v1.y + v2.y;
	v.z = v1.z + v2.z;

	return v;
}

Vector3 VectorLib::Subtract(const Vector3& v1, const Vector3& v2) {
	Vector3 result;
	result.x = v1.x - v2.x;
	result.y = v1.y - v2.y;
	result.z = v1.z - v2.z;
	return result;
}

Vector3 VectorLib::Multiply(float scaler, const Vector3& v)
{
	Vector3 result;

	result.x = scaler * v.x;
	result.y = scaler * v.y;
	result.z = scaler * v.z;

	return result;
}

Vector3 VectorLib::Scaler(const Vector3& v1, const float scaler) {
	Vector3 result;
	result.x = v1.x * scaler;
	result.y = v1.y * scaler;
	result.z = v1.z * scaler;
	return result;
}
#pragma endregion
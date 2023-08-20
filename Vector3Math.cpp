#include "Vector3Math.h"

Vector3 Vector3Math::Add(const Vector3& v1, const Vector3& v2) {
	Vector3 v{};

	v.x = v1.x + v2.x;
	v.y = v1.y + v2.y;
	v.z = v1.z + v2.z;

	return v;
}

Vector3 Vector3Math::Subtract(const Vector3& v1, const Vector3& v2) {
	Vector3 result;
	result.x = v1.x - v2.x;
	result.y = v1.y - v2.y;
	result.z = v1.z - v2.z;
	return result;
}

Vector3 Vector3Math::Scaler(const Vector3& v1, const float scaler) { 
	Vector3 result;
	result.x = v1.x * scaler;
	result.y = v1.y * scaler;
	result.z = v1.z * scaler;
	return result; 
}


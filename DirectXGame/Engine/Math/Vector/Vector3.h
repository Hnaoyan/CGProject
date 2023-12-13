#pragma once
#include <cmath>

struct Vector3 final {

	float x;
	float y;
	float z;

#pragma region 四則演算

	Vector3 operator+(const Vector3& Other) const {
		return Vector3{ this->x + Other.x, this->y + Other.y, this->z + Other.z };
	}
	Vector3 operator-(const Vector3& Other) const {
		return Vector3{ this->x - Other.x, this->y - Other.y, this->z - Other.z };
	}

	Vector3& operator+=(const Vector3& Other) {
		this->x += Other.x;
		this->y += Other.y;
		this->z += Other.z;
		return *this;
	}
	Vector3& operator-=(const Vector3& Other) {
		this->x -= Other.x;
		this->y -= Other.y;
		this->z -= Other.z;
		return *this;
	}

	Vector3 operator*(const Vector3& Other) const {
		return Vector3{ this->x * Other.x, this->y * Other.y, this->z * Other.z };
	}
	Vector3 operator/(const Vector3& Other) const {
		return Vector3{ this->x / Other.x, this->y / Other.y, this->z / Other.z };
	}

	Vector3 operator*(const float& Scaler) const {
		return Vector3{ this->x * Scaler, this->y * Scaler, this->z * Scaler };
	}
	Vector3 operator/(const float& Scaler) const {
		return Vector3{ this->x / Scaler, this->y / Scaler, this->z / Scaler };
	}

	Vector3& operator*=(const float& Scaler) {
		this->x *= Scaler;
		this->y *= Scaler;
		this->z *= Scaler;
		return *this;
	}
	Vector3& operator/=(const float& Scaler) {
		this->x /= Scaler;
		this->y /= Scaler;
		this->z /= Scaler;
		return *this;
	}

#pragma endregion

};


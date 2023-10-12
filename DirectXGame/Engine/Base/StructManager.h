#pragma once

struct Matrix4x4
{
	float m[4][4];
};
struct Matrix3x3
{
	float m[3][3];
};
struct Vector2 {
	float x, y;
};

struct Vector3 {
	float x, y, z;
	Vector3 operator+(const Vector3 v1) {
		Vector3 result{};
		result.x = x + v1.x;
		result.y = y + v1.y;
		result.z = z + v1.z;
		return result;
	}

	Vector3 operator-(const Vector3 v1) {
		Vector3 result{};
		result.x = x - v1.x;
		result.y = y - v1.y;
		result.z = z - v1.z;
		return result;
	}

};

struct Vector4 {
	float x, y, z, w;
};

enum UpDown {
	PatternUp,
	PatternDown,
};

struct ConstBufferDataWorldTransform {
	Matrix4x4 matWorld;
};

struct ConstBufferDataViewProjection {
	Matrix4x4 view;
	Matrix4x4 projection;
	Vector3 cameraPos;
};

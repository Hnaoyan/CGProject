#pragma once

struct Matrix4x4
{
	float m[4][4];
};
struct Matrix3x3
{
	float m[3][3];
};
struct Vector3 {
	float x, y, z;
};

struct Vector4 {
	float x, y, z, w;
};

struct Vector2 {
	float x, y;
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

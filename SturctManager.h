#pragma once

struct Matrix4x4
{
	float m[4][4];
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

struct Transform {
	Vector3 scale;
	Vector3 rotate;
	Vector3 translate;
};

enum UpDown {
	PatternUp,
	PatternDown,
};

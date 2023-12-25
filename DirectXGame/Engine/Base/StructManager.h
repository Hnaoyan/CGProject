#pragma once
#include "Vector/Vector2.h"
#include "Vector/Vector3.h"
#include "Vector/Vector4.h"

struct Matrix4x4
{
	float m[4][4];
};
struct Matrix3x3
{
	float m[3][3];
};

enum UpDown {
	PatternUp,
	PatternDown,
};

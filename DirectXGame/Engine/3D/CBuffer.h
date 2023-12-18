#pragma once
#include "StructManager.h"


struct CBufferDataParitcle {
	Matrix4x4 matWorld;
	Vector3 velocity;
};

struct ConstBufferDataWorldTransform {
	Matrix4x4 matWorld;
};

struct ConstBufferDataViewProjection {
	Matrix4x4 view;
	Matrix4x4 projection;
	Vector3 cameraPos;
};

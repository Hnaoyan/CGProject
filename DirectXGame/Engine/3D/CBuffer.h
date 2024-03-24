#pragma once
#include "StructManager.h"

enum TestParameter {
	kWorldTransform,
	kViewProjection,
	kMaterial,
	kTexture,
	kLight,
};

struct Transform
{
	Vector3 scale;
	Vector3 rotate;
	Vector3 translate;
};
struct VertexData {
	Vector4 position;
	Vector2 texcoord;
	Vector3 normal;
};
struct Material_Test {
	Vector4 color;
	int32_t enableLighting;
	float padding[3];
	Matrix4x4 uvTransform;
};

struct ParticleData_Test {
	Transform transform;
	Vector3 velocity;
	Vector4 color;
	//Emitter emitter;
	float lifeTime;
	float currentTime;
	bool isAlive;
};
//struct ParticleForGPU {
//	Matrix4x4 World;
//	Vector4 Color;
//};
struct Transformmatrix {
	Matrix4x4 WVP;
	Matrix4x4 World;
};

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

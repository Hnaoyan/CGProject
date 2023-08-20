#pragma once
#include "StructManager.h"
#include <wrl.h>
#include <d3d12.h>
#include <numbers>

struct ViewProjection
{
	// 定数バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuff_;

	ConstBufferDataViewProjection* constMap = nullptr;

	Vector3 eye = { 0,0,-50.0f };

	Vector3 target = { 0,0,0 };

	Vector3 up = { 0,1,0 };

	float fovAngleY = 45.0f * (float)(std::numbers::pi / 180.0f);

	float aspectRatio = (float)16 / 9;

	float nearZ = 0.1f;

	float farZ = 1000.0f;

	Matrix4x4 matView;

	Matrix4x4 matProjection;

	void Initialize();

	void CreateConstBuffer();

	void Map();

	void UpdateMatrix();

};


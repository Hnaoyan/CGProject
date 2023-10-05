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

	Vector3 scale_ = { 1.0f,1.0f,1.0f };

	Vector3 rotation_ = { 0,0,0 };

	Vector3 translate_ = { 0,0,-50.0f };

	float fov = 45.0f;

	float fovAngleY = fov * (float)(std::numbers::pi / 180.0f);

	float aspectRatio = (float)16 / 9;

	float nearZ = 0.1f;

	float farZ = 1000.0f;

	Matrix4x4 matView;

	Matrix4x4 matProjection;
	
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 生成
	/// </summary>
	void CreateConstBuffer();

	/// <summary>
	/// マッピング
	/// </summary>
	void Map();

	/// <summary>
	/// 更新
	/// </summary>
	void UpdateMatrix();

	/// <summary>
	/// データ転送
	/// </summary>
	void TransferMatrix();

	/// <summary>
	/// 視野角の設定
	/// </summary>
	/// <param name="range"></param>
	void SetFov(float range) { fov = range; }
};


#pragma once
#include "StructManager.h"
#include <wrl.h>
#include <d3d12.h>

struct WorldTransform
{
	// 定数バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuff_;
	ConstBufferDataWorldTransform* constMap = nullptr;

	// マッピング済みアドレス
	Vector3 scale_ = { 1,1,1 };
	Vector3 rotation_ = { 0,0,0 };
	Vector3 translation_ = { 0,0,0 };
	Matrix4x4 matWorld_;
	WorldTransform* parent_ = nullptr;

	void Initialize();

	void CreateConstBuffer();

	void Map();

	void UpdateMatrix();

	void TransferMatrix();

};


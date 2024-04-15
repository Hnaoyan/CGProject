#pragma once
#include "StructManager.h"

#include <d3d12.h>
#include <string>
#include <wrl.h>

class Material
{
public:

	struct ConstBufferData {
		Vector3 ambient;
		float pad1;
		Vector3 diffuse;
		float pad2;
		Vector3 specular;
		float alpha;
	};

public:

	static Material* Create();

public:
	std::string name_;
	Vector3 ambient_;
	Vector3 diffuse_;
	Vector3 specular_;
	float alpha_;
	std::string textureFilename_;

	Vector4 color_;
	int32_t enableLighting_;
	float padding[3];
	Matrix4x4 uvTransform_;

public:

	ID3D12Resource* GetConstantBuffer() { return constBuff_.Get(); }

	void LoadTexture(const std::string& directoryPath);

	void Update();

	void SetAlpha(float alpha) { alpha_ = alpha; }

	float GetAlpha() { return alpha_; }

	void SetGraphicsCommand(
		ID3D12GraphicsCommandList* commandList, UINT rootParametreIndexMaterial,
		UINT rootParameterIndexTexture);

	void SetGraphicsCommand(
		ID3D12GraphicsCommandList* commandList, UINT rootParametreIndexMaterial,
		UINT rootParameterIndexTexture, uint32_t textureHandle);

private:
	// 定数バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuff_;
	// 定数バッファのマップ
	ConstBufferData* constMap_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0;


private:

	Material() {
		ambient_ = { 0.3f,0.3f,0.3f };
		diffuse_ = { 0.0f,0.0f,0.0f };
		specular_ = { 0.0f,0.0f,0.0f };
		alpha_ = 1.0f;
	}

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 定数バッファ生成
	/// </summary>
	void CreateConstantBuffer();

};


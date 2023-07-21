#pragma once

#include <array>
#include <string>
#include <d3d12.h>
#include "externals/DirectXTex/DirectXTex.h"
#include "externals/DirectXTex/d3dx12.h"

class TextureManager
{
public:
	
	static const size_t kNumDescriptor = 256;

	struct Texture {
		// テクスチャリソース
		Microsoft::WRL::ComPtr<ID3D12Resource> resource;
		// シェーダーリソースビューのハンドル（CPU）
		D3D12_CPU_DESCRIPTOR_HANDLE SrvHandleCPU;
		// シェーダーリソースビューのハンドル（GPU）
		D3D12_GPU_DESCRIPTOR_HANDLE SrvHandleGPU;
		// 名前
		std::string name;
	};

	/// <summary>
	/// 読み込み
	/// </summary>
	/// <param name="fileName"></param>
	/// <returns></returns>
	static uint32_t Load(const std::string& fileName);

	/// <summary>
	/// シングルトンインスタンス
	/// </summary>
	/// <returns></returns>
	static TextureManager* GetInstance();

	/// <summary>
	/// システム初期化
	/// </summary>
	/// <param name="device"></param>
	/// <param name="directoryPath"></param>
	void Initialize(ID3D12Device* device, std::string directoryPath = "Resources/");

	/// <summary>
	/// 全テクスチャリセット
	/// </summary>
	void ResetAll();

	/// <summary>
	/// リソース情報取得
	/// </summary>
	/// <param name="textureHandle"></param>
	/// <returns></returns>
	const D3D12_RESOURCE_DESC GetResourceDesc(uint32_t textureHandle);

	/// <summary>
	/// デスクリプタテーブルをセット
	/// </summary>
	/// <param name="commandList"></param>
	/// <param name="rootParamIndex"></param>
	/// <param name="textureHandle"></param>
	void SetGraphicsRootDescriptorTable(
		ID3D12GraphicsCommandList* commandList, UINT rootParamIndex, uint32_t textureHandle);

private:
	// デバイス
	ID3D12Device* device_ = nullptr;
	// デスクリプタサイズ
	UINT sDescriptorHandleIncrementSize_ = 0u;
	// ディレクトリパス
	std::string directoryPath_;
	// デスクリプタヒープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeap_;
	// 次に使うデスクリプタヒープの番号
	uint32_t indexNextDescriptorHeap_ = 0u;
	// テクスチャコンテナ
	std::array<Texture, kNumDescriptor> textures_;

	/// <summary>
	/// 読み込み設定
	/// </summary>
	/// <param name="fileName"></param>
	/// <returns></returns>
	uint32_t LoadInternal(const std::string& fileName);


private:
	DirectX::ScratchImage LoadTexture(const std::string& filePath);
	ID3D12Resource* CreateTextureResource(ID3D12Device* device, const DirectX::TexMetadata& metadata);

};


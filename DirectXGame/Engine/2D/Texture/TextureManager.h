#pragma once

#include <array>
#include <string>
#include <d3d12.h>
#include "DirectXTex.h"
#include "d3dx12.h"
#include "DirectXCommon.h"

class DirectXCommon;

class TextureManager
{
public:

	static const size_t kNumDescriptor = 256;

	struct Texture {
		// テクスチャリソース
		Microsoft::WRL::ComPtr<ID3D12Resource> resource;
		// シェーダーリソースビューのハンドル（CPU）
		D3D12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV{};
		// シェーダーリソースビューのハンドル（GPU）
		D3D12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV{};
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
	static TextureManager* GetInstance() {
		static TextureManager instance;
		return &instance;
	}

	/// <summary>
	/// システム初期化
	/// </summary>
	/// <param name="device"></param>
	/// <param name="directoryPath"></param>
	void Initialize(DirectXCommon* dxCommon, std::string directoryPath = "Resources/");

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

	uint32_t GetHeapIndex() { return descriptorHeapIndex_; }

	static ID3D12DescriptorHeap* StaticGetDescriptorHeap() { return GetInstance()->descriptorHeap_.Get(); }

	static D3D12_CPU_DESCRIPTOR_HANDLE StaticGetCPUDescriptorHandle(uint32_t index) { return GetInstance()->GetCPUDescriptorHandle(GetInstance()->descriptorHeap_.Get(), GetInstance()->device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV), index); }

	static D3D12_GPU_DESCRIPTOR_HANDLE StaticGetGPUDescriptorHandle(uint32_t index) { return GetInstance()->GetGPUDescriptorHandle(GetInstance()->descriptorHeap_.Get(), GetInstance()->device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV), index); }
	
	//ID3D12DescriptorHeap* StaticGetDescriptorHeap() { return descriptorHeap_.Get(); }

private:
	DirectXCommon* dxCommon_ = nullptr;
	// デバイス
	ID3D12Device* device_ = nullptr;

	//SRV* srv_ = nullptr;
	// デスクリプタサイズ
	UINT sDescriptorHandleIncrementSize_ = 0u;
	// ディレクトリパス
	std::string directoryPath_;
	// デスクリプタヒープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeap_;
	// 次に使うデスクリプタヒープの番号
	uint32_t descriptorHeapIndex_ = 0u;
	// テクスチャコンテナ
	std::array<Texture, kNumDescriptor> textures_;
	//std::array<Microsoft::WRL::ComPtr<ID3D12Resource>, kNumDescriptor> texResources_;
	// テクスチャリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> resource[kNumDescriptor];
	// シェーダーリソースビューのハンドル（CPU）
	D3D12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV[kNumDescriptor];
	// シェーダーリソースビューのハンドル（GPU）
	D3D12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV[kNumDescriptor];
	// 名前
	std::string name[kNumDescriptor];
	Texture texture_[kNumDescriptor];

	/// <summary>
	/// 読み込み設定
	/// </summary>
	/// <param name="fileName"></param>
	/// <returns></returns>
	uint32_t LoadInternal(const std::string& fileName);

	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(ID3D12DescriptorHeap* descriptorHeap, uint32_t descriptorSize, uint32_t index);

	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(ID3D12DescriptorHeap* descriptorHeap, uint32_t descriptorSize, uint32_t index);



private:
	DirectX::ScratchImage LoadTexture(const std::string& filePath);
	//ID3D12Resource* CreateTextureResource(ID3D12Device* device, const DirectX::TexMetadata& metadata);

};
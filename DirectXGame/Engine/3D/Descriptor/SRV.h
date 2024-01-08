#pragma once
#include "TextureManager.h"
#include "DirectXCommon.h"
#include <wrl.h>

class DirectXCommon;

class SRV
{
public:

	void CreateInstancingSRV();

	void StaticInitialize(DirectXCommon* dxCommon);

	ID3D12DescriptorHeap* GetHeap() { return heap_.Get(); }

private:

	DirectXCommon* dxCommon_ = nullptr;

	// デバイス
	ID3D12Device* device_ = nullptr;
	// ヒープ自体
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> heap_;

	uint32_t kDescriptorSize_;
	uint32_t size_;

	D3D12_CPU_DESCRIPTOR_HANDLE handleCPU;
	D3D12_GPU_DESCRIPTOR_HANDLE handleGPU;

};


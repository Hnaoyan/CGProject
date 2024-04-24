#pragma once
#include "TextureManager.h"
#include "DirectXCommon.h"
#include <wrl.h>

class DirectXCommon;

class SRV
{
public:
	static SRV* GetInstance() {
		static SRV instance;
		return &instance;
	}

	static const int max = 1052;

	void CreateInstancingSRV();

	void CreateRenderTexture(ID3D12Resource* resource, D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle, ID3D12Device* device);

	void StaticInitialize(DirectXCommon* dxCommon);

	ID3D12DescriptorHeap* GetHeap() { return heap_.Get(); }

	//static const int max = 1052;
	void Initialize();
	void Update() {};
	void AddIndex() { index_++; }
	uint32_t GetIndex() { return index_; }
	uint32_t GetSizeSRV() { return descriptorSizeSRV_; }
	const D3D12_GPU_DESCRIPTOR_HANDLE GetGPUHandle(uint32_t textureHandle) {
		return handleGPU[textureHandle];
	}
	const D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHandle(uint32_t textureHandle) {
		return handleCPU[textureHandle];
	}
	void SetGPUHandle(ID3D12DescriptorHeap* descriptorheap, uint32_t descriptorSize, uint32_t index) {
		handleGPU[index] = GetSrvHandleGPU(descriptorheap, descriptorSize, index + 1);
	}
	void SetCPUHandle(ID3D12DescriptorHeap* descriptorheap, uint32_t descriptorSize, uint32_t index) {
		handleCPU[index] = GetSrvHandleCPU(descriptorheap, descriptorSize, index + 1);
	}
	void AddPtr(uint32_t index, D3D12_DESCRIPTOR_HEAP_TYPE type);
	D3D12_CPU_DESCRIPTOR_HANDLE GetSrvHandleCPU(ID3D12DescriptorHeap* descriptorheap, uint32_t descriptorSize, uint32_t index);
	D3D12_GPU_DESCRIPTOR_HANDLE GetSrvHandleGPU(ID3D12DescriptorHeap* descriptorheap, uint32_t descriptorSize, uint32_t index);

private:

	DirectXCommon* dxCommon_ = nullptr;

	// デバイス
	ID3D12Device* device_ = nullptr;
	// ヒープ自体
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> heap_;


	//uint32_t kDescriptorSize_;
	//uint32_t size_;

	uint32_t index_ = 0;
	uint32_t descriptorSizeSRV_ = 0;

	D3D12_CPU_DESCRIPTOR_HANDLE handleCPU[max] = {};
	D3D12_GPU_DESCRIPTOR_HANDLE handleGPU[max] = {};

};


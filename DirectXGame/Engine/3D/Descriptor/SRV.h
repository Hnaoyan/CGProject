#pragma once
#include "TextureManager.h"
#include "WindowAPI.h"
#include "NRenderer.h"

class SRV
{
public:

	static void CreateInstancingSRV();

	void StaticInitialize(DirectXDevice* dxDevice);

	ID3D12DescriptorHeap* GetSRV() { return heap_.Get(); }

private:
	DirectXDevice* dxDevice_ = nullptr;
	// デバイス
	ID3D12Device* device_ = nullptr;
	// ヒープ自体
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> heap_;

	uint32_t kDescriptorSize_;
	uint32_t size_;

};


#pragma once
#include "WindowAPI.h"
#include "DirectXCommon.h"

class DSV
{
public:

	void StaticInitialize(DirectXCommon* dxCommon);

	void ClearDepthBuffer(ID3D12GraphicsCommandList* cmdList);

	ID3D12DescriptorHeap* GetDSV() { return heap_.Get(); }

private:

	DirectXCommon* dxCommon_ = nullptr;

	// デバイス
	ID3D12Device* device_ = nullptr;
	// ヒープ自体
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> heap_;

	uint32_t kDescriptorSize_;
	uint32_t size_;

};


#pragma once
#include "WindowAPI.h"
#include "DirectXCommon.h"
#include <vector>

class RTV
{
public:

	static void CreateInstancingRTV();

	void StaticInitialize(DirectXCommon* dxCommon, int32_t bufferWidth, int32_t bufferHeight);

	ID3D12DescriptorHeap* GetRTV() { return heap_.Get(); }

public:
	void CreateSwapChain(int32_t bufferWidth, int32_t bufferHeight);

	void CreateRenderTarget();

	void ClearRenderTarget(ID3D12GraphicsCommandList* cmdList);

private:
	float clearColor[4] = { 0.1f,0.25f,0.5f,1.0f };

	Microsoft::WRL::ComPtr<IDXGISwapChain4> swapChain_;
	std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> backBuffer_;

private:
	DirectXCommon* dxCommon_ = nullptr;
	// デバイス
	ID3D12Device* device_ = nullptr;
	// ヒープ自体
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> heap_;

	uint32_t kDescriptorSize_;
	uint32_t size_;

};


#pragma once
#include <vector>
#include <wrl.h>

#include "NRenderer.h"

class NRenderer;

class RTV
{
public:

	//static void CreateInstancingRTV();

	void StaticInitialize(NRenderer* renderer, int32_t bufferWidth, int32_t bufferHeight);

	void PostDraw();

public:
	ID3D12DescriptorHeap* GetHeap() { return heap_.Get(); }

	ID3D12Resource* GetBackBuffer(UINT index) { return backBuffer_[index].Get(); }

	size_t GetBufferCount() const { return backBuffer_.size(); }

private:
	void CreateSwapChain(int32_t bufferWidth, int32_t bufferHeight);

	void CreateRenderTarget();

public:
	void ClearResourceBarrier(ID3D12GraphicsCommandList* cmdList);

	void ClearRenderTarget(ID3D12GraphicsCommandList* cmdList);

private:
	float clearColor[4] = { 0.1f,0.25f,0.5f,1.0f };

	Microsoft::WRL::ComPtr<IDXGISwapChain4> swapChain_;
	std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> backBuffer_;
	D3D12_CPU_DESCRIPTOR_HANDLE handle_;

public:
	IDXGISwapChain4* GetSwapChain() { return swapChain_.Get(); }
	D3D12_CPU_DESCRIPTOR_HANDLE GetHandle() { return handle_; }
private:
	NRenderer* renderer_ = nullptr;
	// デバイス
	ID3D12Device* device_ = nullptr;
	// ヒープ自体
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> heap_;

	uint32_t kDescriptorSize_;
	uint32_t size_;

	// RTVのデスク
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc_;

};
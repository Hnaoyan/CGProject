#pragma once
#include "DirectXCommon.h"
#include <vector>
#include <wrl.h>

class DirectXCommon;

class RTV
{
public:

	void StaticInitialize(DirectXCommon* dxCommon, int32_t bufferWidth, int32_t bufferHeight);

	void PostDraw();

public: // アクセッサ
	ID3D12DescriptorHeap* GetHeap() { return heap_.Get(); }
	D3D12_RENDER_TARGET_VIEW_DESC GetDesc() { return rtvDesc_; }
	//ID3D12Resource* GetBackBuffer(UINT index) { return backBuffer_[index].Get(); }
	size_t GetBackBufferCount() const { return backBuffer_.size(); }

public:
	void CreateSwapChain();
	void CreateRenderTargetView();
	void ClearRenderTarget(ID3D12GraphicsCommandList* cmdList);

private:
	DirectXCommon* dxCommon_ = nullptr;
	// デバイス
	ID3D12Device* device_ = nullptr;
	// ヒープ自体
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> heap_;

	uint32_t kDescriptorSize_;
	uint32_t size_;

	uint32_t bufferWidth_;
	uint32_t bufferHeight_;

	// RTVのデスク
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc_;
	std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> backBuffer_;
	float clearColor_[4] = { 0.1f,0.25f,0.5f,1.0f };

//private:
//	void CreateSwapChain(int32_t bufferWidth, int32_t bufferHeight);
//
//	void CreateRenderTarget();
//
//public:
//
//	void ClearRenderTarget(ID3D12GraphicsCommandList* cmdList);
//
//private:
//
//	Microsoft::WRL::ComPtr<IDXGISwapChain4> swapChain_;
//	std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> backBuffer_;
//
//public:
//	IDXGISwapChain4* GetSwapChain() { return swapChain_.Get(); }
//

};


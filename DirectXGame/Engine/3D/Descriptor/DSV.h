#pragma once
#include "DirectXCommon.h"
#include <wrl.h>

class DirectXCommon;

class DSV
{
public:
	/// <summary>
	/// DSVの生成
	/// </summary>
	/// <param name="dxCommon"></param>
	void StaticInitialize(DirectXCommon* dxCommon, int32_t bufferWidth, int32_t bufferHeight);

	ID3D12DescriptorHeap* GetHeap() { return heap_.Get(); }

public:
	/// <summary>
	/// DepthBufferの生成
	/// </summary>
	/// <param name="bufferWidth"></param>
	/// <param name="bufferHeight"></param>
	void CreateDepthBuffer();

	/// <summary>
	/// DepthBufferのクリア
	/// </summary>
	/// <param name="cmdList"></param>
	void ClearDepthBuffer(ID3D12GraphicsCommandList* cmdList);


private:

	DirectXCommon* dxCommon_ = nullptr;

	// デバイス
	ID3D12Device* device_ = nullptr;
	// ヒープ自体
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> heap_;

	// インフォ
	uint32_t kDescriptorSize_;
	uint32_t size_;
	uint32_t bufferWidth_;
	uint32_t bufferHeight_;

	// リソース・デスク
	Microsoft::WRL::ComPtr<ID3D12Resource> depthStencilResource_;
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc_{};

private: // 作成用関数
	D3D12_DESCRIPTOR_HEAP_DESC CreateDescriptorHeapDesc(D3D12_DESCRIPTOR_HEAP_TYPE type, UINT numDescriptor, bool shaderVisible);
	D3D12_RESOURCE_DESC CreateResourceDesc(DXGI_FORMAT format, D3D12_RESOURCE_DIMENSION dimension,
		D3D12_RESOURCE_FLAGS flags, uint32_t width, uint32_t height);
	D3D12_HEAP_PROPERTIES CreateHeapProps(D3D12_HEAP_TYPE type);
	D3D12_CLEAR_VALUE CreateClearValue(DXGI_FORMAT format, FLOAT depth);

};


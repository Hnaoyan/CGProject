#include "DSV.h"
#include <cassert>

using namespace Microsoft::WRL;

void DSV::StaticInitialize(DirectXDevice* dxDevice, int32_t bufferWidth, int32_t bufferHeight)
{
	dxDevice_ = dxDevice;
	device_ = dxDevice_->GetDevice();

	CreateDepthBuffer(bufferWidth, bufferHeight);
}

void DSV::CreateDepthBuffer(int32_t bufferWidth, int32_t bufferHeight)
{
	// リソースの設定
	D3D12_RESOURCE_DESC resourceDesc = CreateResourceDesc(
		DXGI_FORMAT_D32_FLOAT, D3D12_RESOURCE_DIMENSION_TEXTURE2D, D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL, bufferWidth, bufferHeight);
	resourceDesc.MipLevels = 0;

	// Heapの設定
	D3D12_HEAP_PROPERTIES heapProp = CreateHeapProps(D3D12_HEAP_TYPE_DEFAULT);

	// 深度値
	D3D12_CLEAR_VALUE depthClearValue = CreateClearValue(DXGI_FORMAT_D32_FLOAT, 1.0f);

	// Resourceの生成
	ComPtr<ID3D12Resource> resource;
	HRESULT result = S_FALSE;
	result = device_->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resourceDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&depthClearValue,
		IID_PPV_ARGS(&resource));
	assert(SUCCEEDED(result));

	// DepthStencilResourceの設定
	depthStencilResource_ = resource;
	// DSV用のヒープ
	D3D12_DESCRIPTOR_HEAP_DESC descriptorHeapDesc = CreateDescriptorHeapDesc(D3D12_DESCRIPTOR_HEAP_TYPE_DSV, 1, false);
	result = device_->CreateDescriptorHeap(&descriptorHeapDesc, IID_PPV_ARGS(&heap_));
	assert(SUCCEEDED(result));

	// DSVDescの設定
	D3D12_DEPTH_STENCIL_VIEW_DESC depthStencilDesc{};
	depthStencilDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthStencilDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	// DSVHeapの先頭にDSVをつくる
	device_->CreateDepthStencilView(depthStencilResource_.Get(), &depthStencilDesc, heap_->GetCPUDescriptorHandleForHeapStart());

}

void DSV::ClearDepthBuffer(ID3D12GraphicsCommandList* cmdList)
{
	// ハンドル
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = heap_->GetCPUDescriptorHandleForHeapStart();
	// 指定した深度で画面全体をクリアする
	cmdList->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

}

D3D12_DESCRIPTOR_HEAP_DESC DSV::CreateDescriptorHeapDesc(D3D12_DESCRIPTOR_HEAP_TYPE type, UINT numDescriptor, bool shaderVisible)
{
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc{};
	heapDesc.Type = type;
	heapDesc.NumDescriptors = numDescriptor;
	heapDesc.Flags = shaderVisible ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	return heapDesc;
}

D3D12_RESOURCE_DESC DSV::CreateResourceDesc(DXGI_FORMAT format, D3D12_RESOURCE_DIMENSION dimension, D3D12_RESOURCE_FLAGS flags, uint32_t width, uint32_t height)
{
	D3D12_RESOURCE_DESC resourceDesc{};

	resourceDesc.Width = width;		// テクスチャの幅
	resourceDesc.Height = height;	// テクスチャの高さ
	resourceDesc.MipLevels = 1;		// mipMapの数
	resourceDesc.DepthOrArraySize = 1;	// 奥行き or 配列Textureの配列数
	resourceDesc.Format = format;	// DepthStencilとして両可能なフォーマット
	resourceDesc.SampleDesc.Count = 1;	// サンプリングカウント。1固定
	resourceDesc.Dimension = dimension;	// 2次元
	resourceDesc.Flags = flags;	// DepthStencilとして使う通知

	return resourceDesc;
}

D3D12_HEAP_PROPERTIES DSV::CreateHeapProps(D3D12_HEAP_TYPE type)
{
	// 利用するHeapの設定
	D3D12_HEAP_PROPERTIES heapProperties{};
	heapProperties.Type = type;
	heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	heapProperties.CreationNodeMask = 1;
	heapProperties.VisibleNodeMask = 1;
	return heapProperties;
}

D3D12_CLEAR_VALUE DSV::CreateClearValue(DXGI_FORMAT format, FLOAT depth)
{
	// 深度値のクリア設定
	D3D12_CLEAR_VALUE depthClearValue{};
	depthClearValue.DepthStencil.Depth = depth;
	depthClearValue.Format = format;
	return depthClearValue;
}
#include "RTV.h"
#include "DescriptorManager.h"
#include <cassert>
#include "SRV.h"

using namespace Microsoft::WRL;

uint32_t RTV::sNextIndexDescpritorHeap_ = 0u;

void RTV::StaticInitialize(DirectXCommon* dxCommon, int32_t bufferWidth, int32_t bufferHeight)
{
	HRESULT result = S_FALSE;
	// 初期化
	dxCommon_ = dxCommon;
	device_ = dxCommon_->GetDevice();
	bufferWidth_ = bufferWidth;
	bufferHeight_ = bufferHeight;

	D3D12_DESCRIPTOR_HEAP_DESC rtvDescriptorHeapDesc{};
	rtvDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvDescriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	rtvDescriptorHeapDesc.NumDescriptors = kNumDescriptor;
	//rtvDescriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

	result = device_->CreateDescriptorHeap(&rtvDescriptorHeapDesc, IID_PPV_ARGS(&heap_));
	assert(SUCCEEDED(result));

}

void RTV::PostDraw()
{
	ID3D12GraphicsCommandList* cmdList = dxCommon_->GetCommandList();
	HRESULT result = S_FALSE;

	// これから書き込むバックバッファのインデックスを取得
	UINT backBufferIndex = dxCommon_->GetSwapChain()->GetCurrentBackBufferIndex();
	// Barrier
	D3D12_RESOURCE_BARRIER barrier = DescriptorManager::GetBarrier(backBuffer_[backBufferIndex].Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);

	cmdList->ResourceBarrier(1, &barrier);

	// コマンドリストの内容を確定させる。全てのコマンドを積んでからCloseすること
	result = cmdList->Close();
	assert(SUCCEEDED(result));
}

Microsoft::WRL::ComPtr<ID3D12Resource> RTV::CreateRenderTextureResource(uint32_t width, uint32_t height, DXGI_FORMAT format, const Vector4& clearColor)
{
	D3D12_RESOURCE_DESC resourceDesc{};
	// RenderTarget
	resourceDesc.Width = width;
	resourceDesc.Height = height;
	resourceDesc.MipLevels = 1;		// mipMapの数
	resourceDesc.DepthOrArraySize = 1;	// 奥行き or 配列Textureの配列数
	resourceDesc.Format = format;	// DepthStencilとして両可能なフォーマット
	resourceDesc.SampleDesc.Count = 1;	// サンプリングカウント。1固定
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;	// 2次元
	resourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;

	D3D12_HEAP_PROPERTIES heapProperties{};
	// VRAM上に
	heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;
	//heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	//heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	//heapProperties.CreationNodeMask = 1;
	//heapProperties.VisibleNodeMask = 1;

	D3D12_CLEAR_VALUE clearValue;
	clearValue.Format = format;
	clearValue.Color[0] = clearColor.x;
	clearValue.Color[1] = clearColor.y;
	clearValue.Color[2] = clearColor.z;
	clearValue.Color[3] = clearColor.w;

	//Microsoft::WRL::ComPtr<ID3D12Resource> resultResource;
	ID3D12Resource* resultResource = nullptr;
	device_->CreateCommittedResource(
		&heapProperties,
		D3D12_HEAP_FLAG_NONE,
		&resourceDesc,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		//D3D12_RESOURCE_STATE_RENDER_TARGET,
		&clearValue,
		IID_PPV_ARGS(&resultResource)
	);
	Microsoft::WRL::ComPtr<ID3D12Resource> result = resultResource;

	return result;
}

void RTV::CreateSwapChain()
{
	HRESULT result = S_FALSE;
	IDXGISwapChain4* swapChainPtr = dxCommon_->GetSwapChain();
	// SwapChainDesc作成
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
	// 画面サイズ
	swapChainDesc.Width = bufferWidth_;
	swapChainDesc.Height = bufferHeight_;
	// 色の形式
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	// マルチサンプルの有無
	swapChainDesc.SampleDesc.Count = 1;
	// 描画ターゲットとして
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 2;	// ダブルバッファ
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;	// モニタにうつしたら破棄

	// 設定を渡すため
	ComPtr<IDXGISwapChain1> swapChain1;

	// SwapChain1で生成
	result = dxCommon_->GetDxgiFactory()->CreateSwapChainForHwnd(
		dxCommon_->GetCommandQueue(),
		WindowAPI::GetInstance()->GetHwnd(),
		&swapChainDesc,
		nullptr, nullptr,
		&swapChain1);
	assert(SUCCEEDED(result));

	// SwapChain1を変換
	result = swapChain1->QueryInterface(IID_PPV_ARGS(&swapChainPtr));
	assert(SUCCEEDED(result));
}

void RTV::CreateRenderTargetView()
{
	HRESULT result = S_FALSE;

	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
	result = dxCommon_->GetSwapChain()->GetDesc(&swapChainDesc);
	assert(SUCCEEDED(result));

	// RTVデスクの設定
	rtvDesc_.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;	// 出力結果をSRGBに変換して書き込む
	rtvDesc_.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;	// 2Dテクスチャとして書き込む

	// 裏表の2つ分
	backBuffer_.resize(swapChainDesc.BufferCount);
	for (int i = 0; i < backBuffer_.size(); i++) {
		// スワップチェーンからバッファ取得
		result = dxCommon_->GetSwapChain()->GetBuffer(i, IID_PPV_ARGS(&backBuffer_[i]));
		assert(SUCCEEDED(result));


		//D3D12_CPU_DESCRIPTOR_HANDLE handle = DescriptorManager::GetCPUDescriptorHandle(heap_.Get(), device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV), i);
		D3D12_CPU_DESCRIPTOR_HANDLE handle = GetCPUDescriptorHandle();
		NextIndex();
		device_->CreateRenderTargetView(backBuffer_[i].Get(), &rtvDesc_, handle);
	}

}

void RTV::CreateRenderTexture(SRV* srv)
{
	// OffScreenRendering
	//D3D12_CPU_DESCRIPTOR_HANDLE handle = DescriptorManager::GetCPUDescriptorHandle(heap_.Get(), device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV), 0);
	D3D12_CPU_DESCRIPTOR_HANDLE handle = GetCPUDescriptorHandle();
	NextIndex();

	rtvDesc_.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;	// 出力結果をSRGBに変換して書き込む
	rtvDesc_.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;	// 2Dテクスチャとして書き込む

	renderTextureResource_ = CreateRenderTextureResource(bufferWidth_, bufferHeight_, DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, kRenderTargetClearValue_);
	device_->CreateRenderTargetView(renderTextureResource_.Get(), &rtvDesc_, handle);
	//SRVの作成
		//D3D12_CPU_DESCRIPTOR_HANDLE handleSrv = DescriptorManager::GetCPUDescriptorHandle(heap_.Get(), device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV), 3);
	D3D12_SHADER_RESOURCE_VIEW_DESC renderTextureSrvDesc{};
	renderTextureSrvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	renderTextureSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	renderTextureSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	renderTextureSrvDesc.Texture2D.MipLevels = 1;
	D3D12_CPU_DESCRIPTOR_HANDLE srvHandle = DescriptorManager::GetCPUDescriptorHandle(srv->GetHeap(), device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV), 2);
	device_->CreateShaderResourceView(renderTextureResource_.Get(), &renderTextureSrvDesc, srvHandle);
	//srv->Create(renderTextureResource_.Get(), handle, device_);

}

void RTV::ClearRenderTarget(ID3D12GraphicsCommandList* cmdList)
{
	// これから書き込むバックバッファのインデックスを取得
	UINT backBufferIndex = dxCommon_->GetSwapChain()->GetCurrentBackBufferIndex();
	
	// 描画先のRTVを設定する
	// ハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = DescriptorManager::GetCPUDescriptorHandle(heap_.Get(), device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV), backBufferIndex);
	
	// 指定した色で画面全体をクリアする
	cmdList->ClearRenderTargetView(rtvHandle, clearColor_, 0, nullptr);	
}

void RTV::ClearRenderTexture(ID3D12GraphicsCommandList* cmdList)
{
	// これから書き込むバックバッファのインデックスを取得
	//UINT backBufferIndex = dxCommon_->GetSwapChain()->GetCurrentBackBufferIndex();

	// 描画先のRTVを設定する
	// ハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = DescriptorManager::GetCPUDescriptorHandle(heap_.Get(), device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV), 2);

	// 指定した色で画面全体をクリアする
	float color[4] = { kRenderTargetClearValue_.x,kRenderTargetClearValue_.y,kRenderTargetClearValue_.z,kRenderTargetClearValue_.w };
	cmdList->ClearRenderTargetView(rtvHandle, color, 0, nullptr);
}

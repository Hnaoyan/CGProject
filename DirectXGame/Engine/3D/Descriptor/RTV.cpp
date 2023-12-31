#include "RTV.h"
#include "DescriptorManager.h"
#include <cassert>

using namespace Microsoft::WRL;

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
	rtvDescriptorHeapDesc.NumDescriptors = 1;
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

	// RTVヒープの生成
	D3D12_DESCRIPTOR_HEAP_DESC rtvDescHeapDesc{};
	rtvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvDescHeapDesc.NumDescriptors = swapChainDesc.BufferCount;
	result = device_->CreateDescriptorHeap(&rtvDescHeapDesc, IID_PPV_ARGS(&heap_));
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


		D3D12_CPU_DESCRIPTOR_HANDLE handle = DescriptorManager::GetCPUDescriptorHandle(heap_.Get(), device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV), i);

		device_->CreateRenderTargetView(backBuffer_[i].Get(), &rtvDesc_, handle);
	}
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

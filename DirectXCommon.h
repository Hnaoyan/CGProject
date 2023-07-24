
#pragma once

#include <Windows.h>
#include <cstdlib>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <chrono>

#include <wrl.h>
#include "WinApp.h"

#include <dxcapi.h>
#pragma comment(lib, "dxcompiler.lib")

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

class DirectXCommon
{
public:
	/// <summary>
	/// シングルトンインスタンス
	/// </summary>
	/// <returns>インスタンス化</returns>
	static DirectXCommon* GetInstance();

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="winApp">winAppの受け取り</param>
	/// <param name="bufferWidth">画面の横幅</param>
	/// <param name="bufferHeight">画面の縦幅</param>
	void Initialize(WinApp* winApp, int32_t bufferWidth = WinApp::kClientWidth, int32_t bufferHeight = WinApp::kClientHeight);

	/// <summary>
	/// 描画前処理
	/// </summary>
	void PreDraw();

	/// <summary>
	/// 描画後処理
	/// </summary>
	void PostDraw();

	/// <summary>
	/// レンダーターゲットのクリア
	/// </summary>
	void ClearRenderTarget();

	/// <summary>
	/// 深度バッファのクリア
	/// </summary>
	void ClearDepthBuffer();

	ID3D12Device* GetDevice() { return device_.Get(); }

	ID3D12GraphicsCommandList* GetCommandList() { return commandList_.Get(); }

	size_t GetBackBufferCount() const { return backBuffer_.size(); }

private:	//メンバ関数

	/// <summary>
	/// CPUDescriptorハンドルの作成
	/// </summary>
	/// <param name="descriptorHeap"></param>
	/// <param name="descriptorSize"></param>
	/// <param name="index"></param>
	/// <returns></returns>
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(ID3D12DescriptorHeap* descriptorHeap, uint32_t descriptorSize, uint32_t index);

	/// <summary>
	/// GPUDescriptorハンドルの作成
	/// </summary>
	/// <param name="descriptorHeap"></param>
	/// <param name="descriptorSize"></param>
	/// <param name="index"></param>
	/// <returns></returns>
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(ID3D12DescriptorHeap* descriptorHeap, uint32_t descriptorSize, uint32_t index);

	/// <summary>
	/// リソースバリアの作成関数
	/// </summary>
	/// <param name="backBuffer"></param>
	/// <param name="stateBefore"></param>
	/// <param name="stateAfter"></param>
	/// <returns></returns>
	D3D12_RESOURCE_BARRIER GetBarrier(ID3D12Resource* backBuffer, D3D12_RESOURCE_STATES stateBefore, D3D12_RESOURCE_STATES stateAfter);

	D3D12_HEAP_PROPERTIES HeapProperties(D3D12_HEAP_TYPE type);

	D3D12_CLEAR_VALUE ClearValue(DXGI_FORMAT format, FLOAT depth);

	D3D12_RESOURCE_DESC GetResoruceHeap(DXGI_FORMAT format, D3D12_RESOURCE_DIMENSION dimension, D3D12_RESOURCE_FLAGS flags, uint32_t width, uint32_t height);

	/// <summary>
	/// Viewport作成関数
	/// </summary>
	D3D12_VIEWPORT CreateViewport(FLOAT width, FLOAT height, FLOAT topLeftX, FLOAT topLeftY, FLOAT minDepth, FLOAT maxDepth);

	/// <summary>
	/// シザー作成関数
	/// </summary>
	D3D12_RECT CreateScissorRect(FLOAT left, FLOAT right, FLOAT top, FLOAT bottom);

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> CreateDescriptorHeap(
		D3D12_DESCRIPTOR_HEAP_TYPE heapType, UINT numDescriptors, bool shaderVisible);

private:	// メンバ変数
	// ウィンドウズアプリケーション管理
	WinApp* winApp_;

	// Direct3D関連のポインタ
	Microsoft::WRL::ComPtr<IDXGIFactory7> dxgiFactory_;
	Microsoft::WRL::ComPtr<ID3D12Device> device_;
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue_;
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> commandAllocator_;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList_;
	Microsoft::WRL::ComPtr<IDXGISwapChain4> swapChain_;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> rtvDescriptorHeap_;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> dsvDescriptorHeap_;
	std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> backBuffer_;
	Microsoft::WRL::ComPtr<ID3D12Resource> depthResourceBuffer_;
	Microsoft::WRL::ComPtr<ID3D12Fence> fence_;

	int32_t backBufferWidth_ = 0;
	int32_t backBufferHeight_ = 0;
	UINT fenceVal_ = 0;

private:	// メンバ関数
	DirectXCommon() = default;
	~DirectXCommon() = default;


	/// <summary>
	/// DXGIデバイスの初期化
	/// </summary>
	void InitializeDXGIDevice();

	/// <summary>
	/// コマンドの初期化
	/// </summary>
	void InitializeCommand();

	/// <summary>
	/// SwapChainの生成
	/// </summary>
	void CreateSwapChain();

	/// <summary>
	/// RTVの生成
	/// </summary>
	void CreateRenderTargetView();

	/// <summary>
	/// 深度Bufferの生成
	/// </summary>
	void CreateDepthBuffer();

	/// <summary>
	/// フェンスの生成
	/// </summary>
	void CreateFence();


};


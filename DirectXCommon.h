#pragma once

#include <Windows.h>
#include <cstdlib>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <cassert>
#include <chrono>

#include <wrl.h>
#include "WinApp.h"

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

private:

	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(ID3D12DescriptorHeap* descriptorHeap, uint32_t descriptorSize, uint32_t index);

	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(ID3D12DescriptorHeap* descriptorHeap, uint32_t descriptorSize, uint32_t index);

	D3D12_RESOURCE_BARRIER GetBarrier(ID3D12Resource* backBuffer, D3D12_RESOURCE_STATES stateBefore, D3D12_RESOURCE_STATES stateAfter);

private:
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
	std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> backBuffer_;
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

	void CreateFence();


};


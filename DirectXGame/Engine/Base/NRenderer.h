#pragma once
// ライブラリ
#include <memory>
#include <wrl.h>
#include <Windows.h>
#include <cstdlib>
#include <chrono>

// ヘッダーファイル
#include "DescriptorManager.h"
#include "DirectXDevice.h"

// DirectX
#include <d3d12.h>
#include <dxgi1_6.h>
#include <dxcapi.h>
#pragma comment(lib, "dxcompiler.lib")

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

class DescriptorManager;
class DirectXDevice;

class DSV;
class SRV;
class RTV;

class NRenderer
{
public:
	NRenderer();
	~NRenderer() = default;

	static NRenderer* GetInstance() {
		static NRenderer instance;
		return &instance;
	}

	void Initialize(WindowAPI* winApp, int32_t bufferWidth = WindowAPI::kClientWidth, int32_t bufferHeight = WindowAPI::kClientHeight);

	void PreDraw();

	void PostDraw();

private:
	void InitializeCommand();
	void CreateFence();

private:
	// ウインドウズAPI
	WindowAPI* winApp_ = nullptr;
	ID3D12Device* device_ = nullptr;

	std::unique_ptr<DescriptorManager> descriptorManager_;
	std::unique_ptr<DirectXDevice> directXDevice_;

	Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue_;
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> commandAllocator_;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList_;
	Microsoft::WRL::ComPtr<ID3D12Fence> fence_;

	uint32_t backBufferWidth_ = 0;
	uint32_t backBufferHeight_ = 0;
	UINT fenceVal_ = 0;

	std::unique_ptr<SRV> srvHeap_;
	std::unique_ptr<DSV> dsvHeap_;
	std::unique_ptr<RTV> rtvHeap_;

public:

	DescriptorManager* GetDescriptorManager() { return descriptorManager_.get(); }
	DirectXDevice* GetDXDevice() { return directXDevice_.get(); }


	ID3D12CommandQueue* GetCmdQueue() { return commandQueue_.Get(); }
	ID3D12CommandAllocator* GetCmdAllocator() { return commandAllocator_.Get(); }
	ID3D12GraphicsCommandList* GetCmdList() { return commandList_.Get(); }
private:
	/// <summary>
	/// Viewport作成関数
	/// </summary>
	D3D12_VIEWPORT CreateViewport(FLOAT width, FLOAT height, FLOAT topLeftX, FLOAT topLeftY, FLOAT minDepth, FLOAT maxDepth);

	/// <summary>
	/// シザー作成関数
	/// </summary>
	D3D12_RECT CreateScissorRect(FLOAT left, FLOAT right, FLOAT top, FLOAT bottom);

};


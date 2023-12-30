#pragma once
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXTex.h>
#include "DirectXCommon.h"
#include "SRV.h"
#include "DSV.h"
#include "RTV.h"

class DescriptorManager
{
public:
	// シングルトン
	static DescriptorManager* GetInstance() {
		static DescriptorManager instance;
		return &instance;
	}

public:
	/// <summary>
	/// ヒープの取得
	/// </summary>
	/// <returns></returns>
	ID3D12DescriptorHeap* GetDescriptorHeap() { return descriptorHeap_.Get(); }

	// 静的初期化
	void StaticInitialize();

	void PreDraw();

	void Finalize();

	// テクスチャ用リセット関数
	void ResetDescriptor(ID3D12Device* device, UINT numSize);
private:
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeap_;

	DirectXCommon* dxCommon_ = nullptr;

public:

	static D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(ID3D12DescriptorHeap* descriptorHeap, uint32_t descriptorSize, uint32_t index);
	static D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(ID3D12DescriptorHeap* descriptorHeap, uint32_t descriptorSize, uint32_t index);
	static D3D12_RESOURCE_BARRIER GetBarrier(ID3D12Resource* backBuffer, D3D12_RESOURCE_STATES stateBefore, D3D12_RESOURCE_STATES stateAfter);

};


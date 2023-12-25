#pragma once
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXTex.h>
//#include "DirectXCommon.h"
#include "SRV.h"
#include "DSV.h"
#include "RTV.h"
#include <memory>
#include "NRenderer.h"

class DescriptorManager
{
public:
	// シングルトン
	static DescriptorManager* GetInstance() {
		static DescriptorManager instance;
		return &instance;
	}

	DescriptorManager();
	~DescriptorManager() = default;

public:
	/// <summary>
	/// ヒープの取得
	/// </summary>
	/// <returns></returns>
	ID3D12DescriptorHeap* GetDescriptorHeap() { return descriptorHeap_.Get(); }

	// SRV
	SRV* GetSRV() { return srvHeap_.get(); }
	ID3D12DescriptorHeap* GetSRVHeap() { return srvHeap_.get()->GetSRV(); }
	// DSV
	DSV* GetDSV() { return dsvHeap_.get(); }
	ID3D12DescriptorHeap* GetDSVHeap() { return dsvHeap_.get()->GetDSV(); }
	// RTV
	RTV* GetRTV() { return rtvHeap_.get(); }
	ID3D12DescriptorHeap* GetRTVHeap() { return rtvHeap_.get()->GetRTV(); }

	// 静的初期化
	void StaticInitialize();

	void PreDraw(ID3D12GraphicsCommandList* cmdList);

	void PostDraw();

	void Finalize();

	// テクスチャ用リセット関数
	void ResetDescriptor(ID3D12Device* device, UINT numSize);
private:
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeap_;

	NRenderer* renderer_ = nullptr;
	//Microsoft::WRL::ComPtr <SRV> srvHeap_;

	std::unique_ptr<SRV> srvHeap_;
	std::unique_ptr<DSV> dsvHeap_;
	std::unique_ptr<RTV> rtvHeap_;

public:

	static D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(ID3D12DescriptorHeap* descriptorHeap, uint32_t descriptorSize, uint32_t index);
	static D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(ID3D12DescriptorHeap* descriptorHeap, uint32_t descriptorSize, uint32_t index);
	static D3D12_RESOURCE_BARRIER GetBarrier(ID3D12Resource* backBuffer, D3D12_RESOURCE_STATES stateBefore, D3D12_RESOURCE_STATES stateAfter);

};

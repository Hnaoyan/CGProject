#pragma once
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXTex.h>
#include "DirectXCommon.h"
#include "Descriptor/SRV.h"
#include <memory>

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

	ID3D12DescriptorHeap* GetSRVHeap() { return srvHeap_.get()->GetSRV(); }

	void StaticInitialize();

	void Finalize();

	// テクスチャ用リセット関数
	void ResetDescriptor(ID3D12Device* device, UINT numSize);
private:
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeap_;

	DirectXCommon* dxCommon_ = nullptr;

	//Microsoft::WRL::ComPtr <SRV> srvHeap_;

	std::unique_ptr<SRV> srvHeap_;

};


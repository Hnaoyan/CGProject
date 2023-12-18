#pragma once
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXTex.h>

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

	// テクスチャ用リセット関数
	void ResetDescriptor(ID3D12Device* device, UINT numSize);

private:
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeap_;

};


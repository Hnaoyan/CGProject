#pragma once
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXTex.h>

class DescriptorManager
{
public:
	static DescriptorManager* GetInstance() {
		static DescriptorManager instance;
		return &instance;
	}

public:
	ID3D12DescriptorHeap* GetDescriptorHeap() { return descriptorHeap_.Get(); }


private:
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeap_;

};


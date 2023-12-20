#include "DescriptorManager.h"

void DescriptorManager::StaticInitialize()
{
	dxCommon_ = DirectXCommon::GetInstance();
	srvHeap_ = std::make_unique<SRV>();
	srvHeap_->StaticInitialize(dxCommon_);

}

void DescriptorManager::Finalize()
{
	srvHeap_->GetSRV()->Release();
}

void DescriptorManager::ResetDescriptor(ID3D12Device* device, UINT numSize)
{
	HRESULT result = S_FALSE;

	// デスクリプタヒープを生成
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	descHeapDesc.NumDescriptors = numSize;
	result = device->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&descriptorHeap_));
	assert(SUCCEEDED(result));

}

#include "SRV.h"
#include "CBuffer.h"

void SRV::CreateInstancingSRV()
{

	D3D12_SHADER_RESOURCE_VIEW_DESC instancingSrvDesc{};

	instancingSrvDesc.Format = DXGI_FORMAT_UNKNOWN;
	instancingSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	instancingSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	instancingSrvDesc.Buffer.FirstElement = 0;
	instancingSrvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	instancingSrvDesc.Buffer.NumElements = 10;
	instancingSrvDesc.Buffer.StructureByteStride = sizeof(CBufferDataParitcle);

	//D3D12_CPU_DESCRIPTOR_HANDLE instSrvHandleCPU;
	//handleCPU =
	//	DescriptorManager::GetCPUDescriptorHandle(
	//		heap_.Get(),
	//		dxCommon_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV),
	//		0);
	//handleGPU =
	//	DescriptorManager::GetGPUDescriptorHandle(
	//		heap_.Get(),
	//		dxCommon_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV),
	//		0);

	//DirectXCommon::GetInstance()->GetDevice()->CreateShaderResourceView(
		//instancingResource_.Get(), &instancingDesc, SrvHandleCPU);

}

void SRV::CreateRenderTexture(ID3D12Resource* resource, D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle,ID3D12Device* device)
{
	// SRVの設定
	D3D12_SHADER_RESOURCE_VIEW_DESC renderTextureSrvDesc{};
	renderTextureSrvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	renderTextureSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	renderTextureSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	renderTextureSrvDesc.Texture2D.MipLevels = 1;

	// SRV生成
	device->CreateShaderResourceView(resource, &renderTextureSrvDesc, cpuHandle);

}

void SRV::StaticInitialize(DirectXCommon* dxCommon)
{
	HRESULT result = S_FALSE;
	dxCommon_ = dxCommon;
	device_ = dxCommon_->GetDevice();

	D3D12_DESCRIPTOR_HEAP_DESC srvDescriptorHeapDesc{};
	srvDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvDescriptorHeapDesc.NumDescriptors = 10000;
	srvDescriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

	result = device_->CreateDescriptorHeap(&srvDescriptorHeapDesc, IID_PPV_ARGS(&heap_));
	assert(SUCCEEDED(result));

	Initialize();
}

void SRV::Initialize()
{
	descriptorSizeSRV_ = dxCommon_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	index_ = 3;
}

void SRV::AddPtr(uint32_t index, D3D12_DESCRIPTOR_HEAP_TYPE type)
{
	handleCPU[index].ptr += dxCommon_->GetDevice()->GetDescriptorHandleIncrementSize(type);
	handleGPU[index].ptr += dxCommon_->GetDevice()->GetDescriptorHandleIncrementSize(type);

}

D3D12_CPU_DESCRIPTOR_HANDLE SRV::GetSrvHandleCPU(ID3D12DescriptorHeap* descriptorheap, uint32_t descriptorSize, uint32_t index)
{
	D3D12_CPU_DESCRIPTOR_HANDLE handle = descriptorheap->GetCPUDescriptorHandleForHeapStart();
	handle.ptr += (descriptorSize * index);
	return handle;
}

D3D12_GPU_DESCRIPTOR_HANDLE SRV::GetSrvHandleGPU(ID3D12DescriptorHeap* descriptorheap, uint32_t descriptorSize, uint32_t index)
{
	D3D12_GPU_DESCRIPTOR_HANDLE handle = descriptorheap->GetGPUDescriptorHandleForHeapStart();
	handle.ptr += (descriptorSize * index);
	return handle;
}

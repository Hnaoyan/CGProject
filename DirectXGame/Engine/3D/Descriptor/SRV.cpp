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
	handleCPU =
		DescriptorManager::GetCPUDescriptorHandle(
			heap_.Get(),
			dxCommon_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV),
			0);
	handleGPU =
		DescriptorManager::GetGPUDescriptorHandle(
			heap_.Get(),
			dxCommon_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV),
			0);

	//DirectXCommon::GetInstance()->GetDevice()->CreateShaderResourceView(
		//instancingResource_.Get(), &instancingDesc, SrvHandleCPU);

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

}

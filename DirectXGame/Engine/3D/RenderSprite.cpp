#include "RenderSprite.h"
#include "../Descriptor/DescriptorManager.h"
#include "../Graphics/PipelineManager.h"
#include "../Descriptor/SRV.h"

void RenderSprite::Initialize(ID3D12Device* device, ID3D12GraphicsCommandList* commandList)
{

	device_ = device;
	commandList_ = commandList;

}

void RenderSprite::Draw(SRV* srv)
{
	// RootSignature
	commandList_->SetPipelineState(PipelineManager::sRenderSpritePipelineStates_.Get());
	commandList_->SetGraphicsRootSignature(PipelineManager::sRenderSpriteRootSignature_.Get());

	// SRV
	ID3D12DescriptorHeap* ppHeap[] = { srv->GetHeap() };
	commandList_->SetDescriptorHeaps(_countof(ppHeap), ppHeap);

	D3D12_GPU_DESCRIPTOR_HANDLE srvGPUHandle = DescriptorManager::GetGPUDescriptorHandle(srv->GetHeap(), D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 0);

	commandList_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	commandList_->SetGraphicsRootDescriptorTable(0, srvGPUHandle);

	commandList_->DrawInstanced(3, 1, 0, 0);

}

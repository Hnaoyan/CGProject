#include "DescriptorManager.h"

DescriptorManager::DescriptorManager()
{
}

void DescriptorManager::StaticInitialize()
{
	renderer_ = NRenderer::GetInstance();
	srvHeap_ = std::make_unique<SRV>();
	dsvHeap_ = std::make_unique<DSV>();
	rtvHeap_ = std::make_unique<RTV>();

	uint32_t width = WindowAPI::kClientWidth;
	uint32_t height = WindowAPI::kClientHeight;
	rtvHeap_->StaticInitialize(renderer_, width, height);
	dsvHeap_->StaticInitialize(renderer_->GetDXDevice(), width, height);
	srvHeap_->StaticInitialize(renderer_->GetDXDevice());

}

void DescriptorManager::PreDraw(ID3D12GraphicsCommandList* cmdList)
{
	rtvHeap_->ClearResourceBarrier(cmdList);
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = dsvHeap_->GetHeap()->GetCPUDescriptorHandleForHeapStart();
	//renderer_->GetCmdList()->OMSetRenderTargets(1, &rtvHeap_->GetHandle(), false, &dsvHandle);
	//cmdList->OMSetRenderTargets(1, &rtvHeap_->GetHandle(), false,&dsvHandle);

	rtvHeap_->ClearRenderTarget(cmdList);
	dsvHeap_->ClearDepthBuffer(cmdList);
}

void DescriptorManager::PostDraw()
{
	rtvHeap_->PostDraw();
}

void DescriptorManager::Finalize()
{
	srvHeap_->GetHeap()->Release();
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

D3D12_CPU_DESCRIPTOR_HANDLE DescriptorManager::GetCPUDescriptorHandle(ID3D12DescriptorHeap* descriptorHeap, uint32_t descriptorSize, uint32_t index)
{
	D3D12_CPU_DESCRIPTOR_HANDLE handleCPU = descriptorHeap->GetCPUDescriptorHandleForHeapStart();
	handleCPU.ptr += (descriptorSize * index);
	return handleCPU;
}

D3D12_GPU_DESCRIPTOR_HANDLE DescriptorManager::GetGPUDescriptorHandle(ID3D12DescriptorHeap* descriptorHeap, uint32_t descriptorSize, uint32_t index)
{
	D3D12_GPU_DESCRIPTOR_HANDLE handleGPU = descriptorHeap->GetGPUDescriptorHandleForHeapStart();
	handleGPU.ptr += (descriptorSize * index);
	return handleGPU;
}

D3D12_RESOURCE_BARRIER DescriptorManager::GetBarrier(ID3D12Resource* backBuffer, D3D12_RESOURCE_STATES stateBefore, D3D12_RESOURCE_STATES stateAfter) {
	// TransitionBarrierの設定
	D3D12_RESOURCE_BARRIER barrier{};
	// 今回のバリアはTransition
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	// Noneにしておく
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	// バリアを張る対象のリソース。
	barrier.Transition.pResource = backBuffer;
	// 遷移前（現在）のResourceState
	barrier.Transition.StateBefore = stateBefore;
	// 遷移後のState
	barrier.Transition.StateAfter = stateAfter;
	return barrier;
}
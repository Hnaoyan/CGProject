#include "DSV.h"

void DSV::StaticInitialize(DirectXCommon* dxCommon)
{
	dxCommon_ = dxCommon;
}

void DSV::ClearDepthBuffer(ID3D12GraphicsCommandList* cmdList)
{
	// ハンドル
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = heap_->GetCPUDescriptorHandleForHeapStart();
	// 指定した深度で画面全体をクリアする
	cmdList->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

}

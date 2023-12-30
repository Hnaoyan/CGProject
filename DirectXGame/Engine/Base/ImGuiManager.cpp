#include "ImGuiManager.h"
#include "imgui_impl_dx12.h"
#include "imgui_impl_win32.h"
#include <cassert>

ImGuiManager* ImGuiManager::GetInstance()
{
	static ImGuiManager instance;
	return &instance;
}

void ImGuiManager::Initialize(DirectXCommon* dxCommon, WindowAPI* winApp)
{
	//HRESULT result = S_FALSE;
	dxCommon_ = dxCommon;
	
	IMGUI_CHECKVERSION();
	// ImGuiのコンテキストを生成
	ImGui::CreateContext();
	// ImGuiのスタイルを設定
	ImGui::StyleColorsDark();
	// プラットフォームとレンダラーのバックエンドを設定
	ImGui_ImplWin32_Init(winApp->GetHwnd());
	ImGui_ImplDX12_Init(dxCommon_->GetDevice(), static_cast<int>(dxCommon_->GetRTV()->GetBackBufferCount()),
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, dxCommon_->GetSRV()->GetHeap(),
		dxCommon_->GetSRV()->GetHeap()->GetCPUDescriptorHandleForHeapStart(),
		dxCommon_->GetSRV()->GetHeap()->GetGPUDescriptorHandleForHeapStart());

	ImGui::CreateContext();
	auto& io = ImGui::GetIO();

	// 
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

}

void ImGuiManager::Begin()
{
	// 開始処理
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void ImGuiManager::End()
{
	// 描画前準備
	ImGui::Render();

}

void ImGuiManager::Draw()
{
	// 描画用のDescriptorHeapの設定
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();
	ID3D12DescriptorHeap* descriptorHeaps[] = { dxCommon_->GetSRV()->GetHeap()};
	commandList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);

	// 実際のcommandListのImGuiの描画コマンドを積む
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), commandList);

}

void ImGuiManager::Finalize()
{
	// ImGuiの後始末
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

}

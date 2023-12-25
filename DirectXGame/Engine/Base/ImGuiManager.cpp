#include "ImGuiManager.h"
#include "imgui_impl_dx12.h"
#include "imgui_impl_win32.h"
#include <cassert>

ImGuiManager* ImGuiManager::GetInstance()
{
	static ImGuiManager instance;
	return &instance;
}

void ImGuiManager::Initialize(NRenderer* render, WindowAPI* winApp)
{
	//HRESULT result = S_FALSE;
	render_ = render;
	descriptor_ = render_->GetDescriptorManager();
	
	IMGUI_CHECKVERSION();
	// ImGuiのコンテキストを生成
	ImGui::CreateContext();
	// ImGuiのスタイルを設定
	ImGui::StyleColorsDark();
	// プラットフォームとレンダラーのバックエンドを設定
	ImGui_ImplWin32_Init(winApp->GetHwnd());
	ImGui_ImplDX12_Init(render_->GetDXDevice()->GetDevice(),
		static_cast<int>(descriptor_->GetRTV()->GetBufferCount()),
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, descriptor_->GetSRVHeap(),
		descriptor_->GetSRVHeap()->GetCPUDescriptorHandleForHeapStart(),
		descriptor_->GetSRVHeap()->GetGPUDescriptorHandleForHeapStart());

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
	ID3D12GraphicsCommandList* commandList = render_->GetCmdList();
	ID3D12DescriptorHeap* descriptorHeaps[] = { descriptor_->GetSRVHeap() };
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

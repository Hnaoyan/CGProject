#pragma once
#include "DirectXCommon.h"
#include "WinApp.h"
#include "imgui_impl_dx12.h"
#include "imgui_impl_win32.h"

class ImGuiManager
{
public:
	/// <summary>
	/// シングルトンインスタンス
	/// </summary>
	/// <returns></returns>
	static ImGuiManager* GetInstance();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="dxCommon"></param>
	/// <param name="winApp"></param>
	void Initialize(DirectXCommon* dxCommon,WinApp* winApp);

	/// <summary>
	/// Imgui開始
	/// </summary>
	void Begin();

	/// <summary>
	/// Imgui終了
	/// </summary>
	void End();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize();

private:

	DirectXCommon* dxCommon_ = nullptr;

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvDescriptorHeap_;

private:
	ImGuiManager() = default;
	~ImGuiManager() = default;


};


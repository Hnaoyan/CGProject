//#include "DirectXCommon.h"
#include "WindowAPI.h"
#include "NRenderer.h"
#include "imgui_impl_dx12.h"
#include "imgui_impl_win32.h"
#include "Descriptor/DescriptorManager.h"

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
	void Initialize(NRenderer* render,WindowAPI* winApp);

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

	NRenderer* render_ = nullptr;
	DescriptorManager* descriptor_ = nullptr;

private:
	ImGuiManager() = default;
	~ImGuiManager() = default;

	ImGuiManager(const ImGuiManager&) = delete;
	const ImGuiManager& operator = (const ImGuiManager&) = delete;

};


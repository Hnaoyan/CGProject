
#pragma once

#include <Windows.h>
#include <cstdlib>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <chrono>
#include <wrl.h>
#include <memory>

#include "WindowAPI.h"
#include "Descriptor/DescriptorManager.h"

#include <dxcapi.h>
#pragma comment(lib, "dxcompiler.lib")

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

class SRV;
class DSV;
class RTV;

class DirectXCommon
{
public:
	/// <summary>
	/// シングルトンインスタンス
	/// </summary>
	/// <returns>インスタンス化</returns>
	static DirectXCommon* GetInstance();

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="winApp">winAppの受け取り</param>
	/// <param name="bufferWidth">画面の横幅</param>
	/// <param name="bufferHeight">画面の縦幅</param>
	void Initialize(WindowAPI* winApp, int32_t bufferWidth = WindowAPI::kClientWidth, int32_t bufferHeight = WindowAPI::kClientHeight);

	/// <summary>
	/// 描画前処理
	/// </summary>
	void PreDraw();

	/// <summary>
	/// 描画後処理
	/// </summary>
	void PostDraw();

	void RenderTexturePreDraw();

	void RenderTexturePostDraw();

	/// <summary>
	/// 深度バッファのクリア
	/// </summary>
	void ClearDepthBuffer();
private:
	// Direct3D関連のポインタ
	Microsoft::WRL::ComPtr<IDXGIFactory7> dxgiFactory_;
	Microsoft::WRL::ComPtr<ID3D12Device> device_;
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue_;
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> commandAllocator_;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList_;

public: // 取得・設定
	/// <summary>
	/// デバイス取得
	/// </summary>
	/// <returns></returns>
	ID3D12Device* GetDevice() { return device_.Get(); }

	/// <summary>
	/// コマンドリスト取得
	/// </summary>
	/// <returns></returns>
	ID3D12GraphicsCommandList* GetCommandList() { return commandList_.Get(); }

	/// <summary>
	/// DXGIファクトリーの取得
	/// </summary>
	/// <returns></returns>
	IDXGIFactory7* GetDxgiFactory() { return dxgiFactory_.Get(); }

	/// <summary>
	/// コマンドキューの取得
	/// </summary>
	/// <returns></returns>
	ID3D12CommandQueue* GetCommandQueue() { return commandQueue_.Get(); }

	IDXGISwapChain4* GetSwapChain() { return swapChain_.Get(); }

	/// <summary>
	/// バッファーカウントの取得
	/// </summary>
	/// <returns></returns>
	//size_t GetBackBufferCount() const { return backBuffer_.size(); }

public:
	SRV* GetSRV() { return srv_.get(); }
	DSV* GetDSV() { return dsv_.get(); }
	RTV* GetRTV() { return rtv_.get(); }
private:	// メンバ変数
	// ウィンドウズアプリケーション管理
	WindowAPI* winApp_;

	Microsoft::WRL::ComPtr<IDXGISwapChain4> swapChain_;
	Microsoft::WRL::ComPtr<ID3D12Fence> fence_;

	//std::unique_ptr<DescriptorManager> descriptorManager_;

	int32_t backBufferWidth_ = 0;
	int32_t backBufferHeight_ = 0;
	UINT fenceVal_ = 0;
	
	std::unique_ptr<SRV> srv_;

	std::unique_ptr<RTV> rtv_;
	//D3D12_RENDER_TARGET_VIEW_DESC rtvDesc_;
	//std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> backBuffer_;

	std::unique_ptr<DSV> dsv_;

private:	// メンバ関数
	DirectXCommon() = default;
	~DirectXCommon() = default;


	/// <summary>
	/// DXGIデバイスの初期化
	/// </summary>
	void InitializeDXGIDevice();

	/// <summary>
	/// コマンドの初期化
	/// </summary>
	void InitializeCommand();

	///// <summary>
	///// SwapChainの生成
	///// </summary>
	void CreateSwapChain();

	/// <summary>
	/// フェンスの生成
	/// </summary>
	void CreateFence();

private:	//メンバ関数

	/// <summary>
	/// Viewport作成関数
	/// </summary>
	D3D12_VIEWPORT CreateViewport(FLOAT width, FLOAT height, FLOAT topLeftX, FLOAT topLeftY, FLOAT minDepth, FLOAT maxDepth);

	/// <summary>
	/// シザー作成関数
	/// </summary>
	D3D12_RECT CreateScissorRect(FLOAT left, FLOAT right, FLOAT top, FLOAT bottom);

private: // メンバ関数
#pragma region フレーム関係
	// 記録時間(FPS固定用)
	std::chrono::steady_clock::time_point reference_;

	/// <summary>
	/// FPS固定初期化
	/// </summary>
	void InitializeFixFPS();

	/// <summary>
	/// FPS固定更新
	/// </summary>
	void UpdateFixFPS();
#pragma endregion
};


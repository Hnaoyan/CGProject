#include "DirectXCommon.h"
#include "Graphics/PipelineManager.h"
#include "DescriptorManager.h"
#include <cassert>
#include <thread>

using namespace Microsoft::WRL;

DirectXCommon* DirectXCommon::GetInstance() {
	static DirectXCommon instance;
	return &instance;
}

void DirectXCommon::Initialize(WindowAPI* winApp, int32_t bufferWidth, int32_t bufferHeight) {
	assert(winApp);
	winApp_ = WindowAPI::GetInstance();
	backBufferWidth_ = bufferWidth;
	backBufferHeight_ = bufferHeight;

	// フレーム固定の初期化
	InitializeFixFPS();

	// DXGIデバイス初期化
	InitializeDXGIDevice();

	// コマンド初期化
	InitializeCommand();

	srv_ = std::make_unique<SRV>();
	srv_->StaticInitialize(this);

	rtv_ = std::make_unique<RTV>();
	rtv_->StaticInitialize(this, backBufferWidth_, backBufferHeight_);
	//rtv_->CreateSwapChain();
	CreateSwapChain();
	rtv_->CreateRenderTargetView();
	//CreateRenderTargetView();

	dsv_ = std::make_unique<DSV>();
	dsv_->StaticInitialize(this, backBufferWidth_, backBufferHeight_);
	dsv_->CreateDepthBuffer();

	// フェンスの生成
	CreateFence();


}

void DirectXCommon::PreDraw()
{
	UINT backBufferIndex = swapChain_->GetCurrentBackBufferIndex();
	// Barrier
	D3D12_RESOURCE_BARRIER barrier = DescriptorManager::GetBarrier(rtv_->GetBackBuffer(backBufferIndex),
		D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
	// TransitionBarrierを張る
	commandList_->ResourceBarrier(1, &barrier);
	// 描画先のRTVを設定する
	// ハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = DescriptorManager::GetCPUDescriptorHandle(rtv_->GetHeap(), device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV), backBufferIndex);
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = dsv_->GetHeap()->GetCPUDescriptorHandleForHeapStart();
	// レンダーターゲットを設定
	commandList_->OMSetRenderTargets(1, &rtvHandle, false, &dsvHandle);

	rtv_->ClearRenderTarget(commandList_.Get());
	dsv_->ClearDepthBuffer(commandList_.Get());
	//ClearDepthBuffer();

	// ビューポートの設定
	D3D12_VIEWPORT viewport = CreateViewport(FLOAT(backBufferWidth_), FLOAT(backBufferHeight_), 0, 0, 0.0f, 1.0f);
	D3D12_RECT scissorRect = CreateScissorRect(0, FLOAT(backBufferWidth_), 0, FLOAT(backBufferHeight_));
	commandList_->RSSetViewports(1, &viewport);
	commandList_->RSSetScissorRects(1, &scissorRect);
}

void DirectXCommon::PostDraw() {
	HRESULT result = S_FALSE;

	//rtv_->PostDraw();

	// これから書き込むバックバッファのインデックスを取得
	UINT backBufferIndex = swapChain_->GetCurrentBackBufferIndex();
	// Barrier
	D3D12_RESOURCE_BARRIER barrier = DescriptorManager::GetBarrier(rtv_->GetBackBuffer(backBufferIndex),
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);

	commandList_->ResourceBarrier(1, &barrier);

	// コマンドリストの内容を確定させる。全てのコマンドを積んでからCloseすること
	result = commandList_->Close();
	assert(SUCCEEDED(result));

	ID3D12CommandList* commandLists[] = { commandList_.Get() };
	commandQueue_->ExecuteCommandLists(1, commandLists);

	// GPUとOSに画面の交換を行うよう通知する
	swapChain_->Present(1, 0);

	// FenceのSignalを待つためのイベントを作成する
	HANDLE fenceEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	assert(fenceEvent != nullptr);


	// GPUがここまでたどり着いたときに、Fenceの値を指定した値に代入するようにSignalを送る
	commandQueue_->Signal(fence_.Get(), ++fenceVal_);

	if (fence_->GetCompletedValue() != fenceVal_)
	{
		// 指定したSignalにたどりついていないので、たどり着くまで待つようにイベントを設定する
		fence_->SetEventOnCompletion(fenceVal_, fenceEvent);
		// イベントを待つ
		WaitForSingleObject(fenceEvent, INFINITE);
		CloseHandle(fenceEvent);
	}

	// フレーム固定の処理
	UpdateFixFPS();

	// 次のフレーム用のコマンドリストを準備
	result = commandAllocator_->Reset();
	assert(SUCCEEDED(result));
	result = commandList_->Reset(commandAllocator_.Get(), nullptr);

}

void DirectXCommon::ClearDepthBuffer()
{
	dsv_->ClearDepthBuffer(this->commandList_.Get());
}

void DirectXCommon::InitializeDXGIDevice() {
	HRESULT result = S_FALSE;

#ifdef _DEBUG
	ComPtr<ID3D12Debug1> debugController = nullptr;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)))) {
		// デバッグレイヤーを有効化する
		debugController->EnableDebugLayer();
		// さらにGPU側でもチェックを行うようにする
		debugController->SetEnableGPUBasedValidation(TRUE);
	}
#endif // _DEBUG

	// 機能レベルとログ出力用の文字列
	D3D_FEATURE_LEVEL featureLevels[] = {
		D3D_FEATURE_LEVEL_12_2,D3D_FEATURE_LEVEL_12_1,D3D_FEATURE_LEVEL_12_0
	};

	result = CreateDXGIFactory1(IID_PPV_ARGS(&dxgiFactory_));
	assert(SUCCEEDED(result));

	// 使用するアダプタ用の変数。
	ComPtr<IDXGIAdapter4> useAdapter;

	// 良い順にアダプタを頼む
	for (UINT i = 0; dxgiFactory_->EnumAdapterByGpuPreference(i,
		DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE,
		IID_PPV_ARGS(&useAdapter)) != DXGI_ERROR_NOT_FOUND; ++i) {
		// アダプターの情報を取得する
		DXGI_ADAPTER_DESC3 adapterDesc{};
		result = useAdapter->GetDesc3(&adapterDesc);
		assert(SUCCEEDED(result));	// 取得できないのは一大事

		// ソフトウェアアダプタでなければ採用！
		if (!(adapterDesc.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE)) {
			break;
		}
		useAdapter = nullptr;	// ソフトウェアアダプタの場合は見なかったことにする
	}
	// 適切なアダプタが見つからなかったので起動できない
	assert(useAdapter != nullptr);

	const char* featureLevelStrings[] = { "12.2","12.1","12.0" };
	// 高い順に生成できるか試していく
	for (size_t i = 0; i < _countof(featureLevels); ++i) {
		// 採用したアダプターでデバイスを生成
		result = D3D12CreateDevice(useAdapter.Get(), featureLevels[i], IID_PPV_ARGS(&device_));
		// 指定した機能レベルでデバイスが生成できたかを確認
		if (SUCCEEDED(result)) {
			// 生成できたのでループを抜ける
			break;
		}
	}
	// デバイスを生成がうまくいかなかったので起動できない
	assert(device_ != nullptr);
	assert(SUCCEEDED(result));

#ifdef _DEBUG
	ComPtr<ID3D12InfoQueue> infoQueue;
	if (SUCCEEDED(device_->QueryInterface(IID_PPV_ARGS(&infoQueue)))) {
		// 抑制するエラー
		D3D12_MESSAGE_ID denyIds[] = {
			/*
			 * Windows11でのDXGIデバッグレイヤーとDX12デバッグレイヤーの相互作用バグによるエラーメッセージ
			 * https://stackoverflow.com/questions/69805245/directx-12-application-is-crashing-in-windows-11
			 */
			D3D12_MESSAGE_ID_RESOURCE_BARRIER_MISMATCHING_COMMAND_LIST_TYPE };
		// 抑制する表示レベル
		D3D12_MESSAGE_SEVERITY severities[] = { D3D12_MESSAGE_SEVERITY_INFO };
		D3D12_INFO_QUEUE_FILTER filter{};
		filter.DenyList.NumIDs = _countof(denyIds);
		filter.DenyList.pIDList = denyIds;
		filter.DenyList.NumSeverities = _countof(severities);
		filter.DenyList.pSeverityList = severities;
		// 指定したエラーの表示を抑制する
		infoQueue->PushStorageFilter(&filter);
		// エラー時にブレークを発生させる
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);
	}
#endif

}

void DirectXCommon::InitializeCommand() {
	HRESULT result = S_FALSE;
	// コマンドキューを生成する
	D3D12_COMMAND_QUEUE_DESC commandQueueDesc{};
	result = device_->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(&commandQueue_));
	// コマンドキューの生成がうまくいかなかったので起動できない
	assert(SUCCEEDED(result));

	// コマンドアロケータを生成する
	result = device_->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocator_));
	// コマンドアロケータの生成がうまくいかなかったので起動できない
	assert(SUCCEEDED(result));

	// コマンドリストを生成する
	result = device_->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator_.Get(), nullptr, IID_PPV_ARGS(&commandList_));
	// コマンドリストの生成がうまくいかなかったので起動できない
	assert(SUCCEEDED(result));

}

void DirectXCommon::CreateSwapChain()
{
	HRESULT result = S_FALSE;
	// SwapChainDesc作成
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
	// 画面サイズ
	swapChainDesc.Width = backBufferWidth_;
	swapChainDesc.Height = backBufferHeight_;
	// 色の形式
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	// マルチサンプルの有無
	swapChainDesc.SampleDesc.Count = 1;
	// 描画ターゲットとして
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 2;	// ダブルバッファ
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;	// モニタにうつしたら破棄

	// 設定を渡すため
	ComPtr<IDXGISwapChain1> swapChain1;

	// SwapChain1で生成
	result = dxgiFactory_->CreateSwapChainForHwnd(
		commandQueue_.Get(),
		WindowAPI::GetInstance()->GetHwnd(),
		&swapChainDesc,
		nullptr, nullptr,
		&swapChain1);
	assert(SUCCEEDED(result));

	// SwapChain1を変換
	result = swapChain1->QueryInterface(IID_PPV_ARGS(&swapChain_));
	assert(SUCCEEDED(result));
}

void DirectXCommon::CreateFence() {
	HRESULT result = S_FALSE;

	// Fenceの生成
	result = device_->CreateFence(fenceVal_, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence_));
	assert(SUCCEEDED(result));

}

void DirectXCommon::InitializeFixFPS()
{
	// 現在時間を記録する
	reference_ = std::chrono::steady_clock::now();

}

void DirectXCommon::UpdateFixFPS()
{
	// 1/60秒ぴったりの時間
	const std::chrono::microseconds kMinTime(uint64_t(1000000.0f / 60.0f));
	// 1/60秒よりわずかに短い時間
	const std::chrono::microseconds kMinCheckTime(uint64_t(1000000.0f / 75.0f));

	// 現在時間を取得
	std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
	// 前回記録からの経過時間を取得する
	std::chrono::microseconds elapsed =
		std::chrono::duration_cast<std::chrono::microseconds>(now - reference_);

	// 1/60秒（よりわずかに短い時間）経っていない場合
	if (elapsed < kMinCheckTime) {
		// 1/60秒経過するまで微小なスリープを繰り返す
		while (std::chrono::steady_clock::now() - reference_ < kMinCheckTime) {
			// 1マイクロ秒スリープ
			std::this_thread::sleep_for(std::chrono::microseconds(1));
		}
	}
	// 現在の時間を記録する
	reference_ = std::chrono::steady_clock::now();

}

D3D12_VIEWPORT DirectXCommon::CreateViewport(FLOAT width, FLOAT height, FLOAT topLeftX, FLOAT topLeftY, FLOAT minDepth, FLOAT maxDepth) {
	D3D12_VIEWPORT viewport{};
	viewport.Width = width;
	viewport.Height = height;
	viewport.TopLeftX = topLeftX;
	viewport.TopLeftY = topLeftY;
	viewport.MinDepth = minDepth;
	viewport.MaxDepth = maxDepth;
	return viewport;
}

D3D12_RECT DirectXCommon::CreateScissorRect(FLOAT left, FLOAT right, FLOAT top, FLOAT bottom) {
	D3D12_RECT scissorRect{};
	scissorRect.left = LONG(left);
	scissorRect.right = LONG(right);
	scissorRect.top = LONG(top);
	scissorRect.bottom = LONG(bottom);
	return scissorRect;
}

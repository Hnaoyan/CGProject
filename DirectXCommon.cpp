#include "DirectXCommon.h"
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

using namespace Microsoft::WRL;

D3D12_CPU_DESCRIPTOR_HANDLE DirectXCommon::GetCPUDescriptorHandle(ID3D12DescriptorHeap* descriptorHeap, uint32_t descriptorSize, uint32_t index)
{
	D3D12_CPU_DESCRIPTOR_HANDLE handleCPU = descriptorHeap->GetCPUDescriptorHandleForHeapStart();
	handleCPU.ptr += (descriptorSize * index);
	return handleCPU;
}

D3D12_GPU_DESCRIPTOR_HANDLE DirectXCommon::GetGPUDescriptorHandle(ID3D12DescriptorHeap* descriptorHeap, uint32_t descriptorSize, uint32_t index)
{
	D3D12_GPU_DESCRIPTOR_HANDLE handleGPU = descriptorHeap->GetGPUDescriptorHandleForHeapStart();
	handleGPU.ptr += (descriptorSize * index);
	return handleGPU;
}

D3D12_RESOURCE_BARRIER DirectXCommon::GetBarrier(ID3D12Resource* backBuffer, D3D12_RESOURCE_STATES stateBefore, D3D12_RESOURCE_STATES stateAfter) {
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

DirectXCommon* DirectXCommon::GetInstance() {
	static DirectXCommon instance;
	return &instance;
}

void DirectXCommon::Initialize(WinApp* winApp, int32_t bufferWidth, int32_t bufferHeight) {
	assert(winApp);
	winApp_ = winApp;
	backBufferWidth_ = bufferWidth;
	backBufferHeight_ = bufferHeight;

	// DXGIデバイス初期化
	InitializeDXGIDevice();

	// コマンド初期化
	InitializeCommand();

	// SwapChainの生成
	CreateSwapChain();

	// RTVの生成
	CreateRenderTargetView();

	// フェンスの生成
	CreateFence();

}

void DirectXCommon::PreDraw() {
	//HRESULT result = S_FALSE;
	// これから書き込むバックバッファのインデックスを取得
	UINT backBufferIndex = swapChain_->GetCurrentBackBufferIndex();

	// Barrier
	D3D12_RESOURCE_BARRIER barrier = GetBarrier(backBuffer_[backBufferIndex].Get(),
		D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
	// TransitionBarrierを張る
	commandList_->ResourceBarrier(1, &barrier);

	// 描画先のRTVを設定する
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = GetCPUDescriptorHandle(rtvDescriptorHeap_.Get(), device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV), backBufferIndex);
	commandList_->OMSetRenderTargets(1, &rtvHandle, false, nullptr);
	// 指定した色で画面全体をクリアする
	float clearColor[] = { 0.1f,0.25f,0.5f,1.0f };
	commandList_->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
}

void DirectXCommon::PostDraw() {
	HRESULT result = S_FALSE;

	// これから書き込むバックバッファのインデックスを取得
	UINT backBufferIndex = swapChain_->GetCurrentBackBufferIndex();
	// Barrier
	D3D12_RESOURCE_BARRIER barrier = GetBarrier(backBuffer_[backBufferIndex].Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);

	commandList_->ResourceBarrier(1, &barrier);

	// コマンドリストの内容を確定させる。全てのコマンドを積んでからCloseすること
	result = commandList_->Close();
	assert(SUCCEEDED(result));

	ID3D12CommandList* commandLists[] = { commandList_.Get()};
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

	}

	// 次のフレーム用のコマンドリストを準備
	result = commandAllocator_->Reset();
	assert(SUCCEEDED(result));
	result = commandList_->Reset(commandAllocator_.Get(), nullptr);


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

void DirectXCommon::CreateSwapChain() {
	HRESULT result = S_FALSE;

	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
	swapChainDesc.Width = backBufferWidth_;	// 画面の幅
	swapChainDesc.Height = backBufferHeight_;// 画面の高さ
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	// 色の形式
	swapChainDesc.SampleDesc.Count = 1;	// マルチサンプルしない
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	// 描画のターゲットとして利用する
	swapChainDesc.BufferCount = 2;	// ダブルバッファ
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;	// モニタにうつしたら、中身を破棄
	// コマンドキュー、ウィンドウハンドル、設定を渡して生成する
	ComPtr<IDXGISwapChain1> swapChain1;

	result = dxgiFactory_->CreateSwapChainForHwnd(
		commandQueue_.Get(),
		winApp_->GetHwnd(),
		&swapChainDesc,
		nullptr, nullptr,
		&swapChain1);
	assert(SUCCEEDED(result));

	result = swapChain1->QueryInterface(IID_PPV_ARGS(&swapChain_));
	assert(SUCCEEDED(result));

}

void DirectXCommon::CreateRenderTargetView() {
	HRESULT result = S_FALSE;

	DXGI_SWAP_CHAIN_DESC swcDesc = {};
	result = swapChain_->GetDesc(&swcDesc);
	assert(SUCCEEDED(result));

	// ディスクリプタヒープの生成
	D3D12_DESCRIPTOR_HEAP_DESC rtvDescriptorHeapDesc{};
	rtvDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;	// レンダーターゲットビュー用
	rtvDescriptorHeapDesc.NumDescriptors = 2;	// ダブルバッファ用に2つ。多くても別に構わない
	result = device_->CreateDescriptorHeap(&rtvDescriptorHeapDesc, IID_PPV_ARGS(&rtvDescriptorHeap_));
	// ディスクリプタヒープが作れなかったので起動できない
	assert(SUCCEEDED(result));

	// 裏表の2つ分について
	backBuffer_.resize(swcDesc.BufferCount);
	for (int i = 0; i < backBuffer_.size(); i++) {
		// スワップチェーンからバッファ取得
		result = swapChain_->GetBuffer(i, IID_PPV_ARGS(&backBuffer_[i]));
		assert(SUCCEEDED(result));

		// RTVディスクリプタのハンドルを取得
		D3D12_CPU_DESCRIPTOR_HANDLE handle = GetCPUDescriptorHandle(rtvDescriptorHeap_.Get(), device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV), i);

		// RTVの設定
		D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
		rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;	// 出力結果をSRGBに変換して書き込む
		rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;	// 2Dテクスチャとして書き込む

		device_->CreateRenderTargetView(backBuffer_[i].Get(), &rtvDesc, handle);
	}
}

void DirectXCommon::CreateFence() {
	HRESULT result = S_FALSE;

	// Fenceの生成
	result = device_->CreateFence(fenceVal_, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence_));
	assert(SUCCEEDED(result));

}
#include "NRenderer.h"

NRenderer::NRenderer()
{

	descriptorManager_ = std::make_unique<DescriptorManager>();
	directXDevice_ = std::make_unique<DirectXDevice>();

}

void NRenderer::Initialize(WindowAPI* winApp, int32_t bufferWidth, int32_t bufferHeight)
{
	assert(winApp);
	winApp_ = winApp;
	backBufferWidth_ = bufferWidth;
	backBufferHeight_ = bufferHeight;
	// ヒープ系の初期化
	descriptorManager_->StaticInitialize();

	// デバイスの生成
	directXDevice_->Initialize();

	// コマンド関係の生成
	InitializeCommand();

	descriptorManager_->GetRTV()->StaticInitialize(this, backBufferWidth_, backBufferHeight_);

	descriptorManager_->GetDSV()->StaticInitialize(directXDevice_.get(), backBufferWidth_, backBufferHeight_);

	descriptorManager_->GetSRV()->StaticInitialize(directXDevice_.get());

	CreateFence();
}

void NRenderer::PreDraw()
{
	descriptorManager_->PreDraw(commandList_.Get());

	// ビューポートの設定
	D3D12_VIEWPORT viewport = CreateViewport(FLOAT(backBufferWidth_), FLOAT(backBufferHeight_), 0, 0, 0.0f, 1.0f);
	D3D12_RECT scissorRect = CreateScissorRect(0, FLOAT(backBufferWidth_), 0, FLOAT(backBufferHeight_));
	commandList_->RSSetViewports(1, &viewport);
	commandList_->RSSetScissorRects(1, &scissorRect);
}

void NRenderer::PostDraw()
{
	HRESULT result = S_FALSE;

	descriptorManager_->PostDraw();

	ID3D12CommandList* commandLists[] = { commandList_.Get() };
	commandQueue_->ExecuteCommandLists(1, commandLists);

	// GPUとOSに画面の交換を行うよう通知する
	descriptorManager_->GetRTV()->GetSwapChain()->Present(1, 0);

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
	//UpdateFixFPS();

	// 次のフレーム用のコマンドリストを準備
	result = commandAllocator_->Reset();
	assert(SUCCEEDED(result));
	result = commandList_->Reset(commandAllocator_.Get(), nullptr);


}

void NRenderer::InitializeCommand()
{
	device_ = directXDevice_->GetDevice();
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

void NRenderer::CreateFence()
{
	HRESULT result = S_FALSE;

	// Fenceの生成
	result = device_->CreateFence(fenceVal_, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence_));
	assert(SUCCEEDED(result));
}

D3D12_VIEWPORT NRenderer::CreateViewport(FLOAT width, FLOAT height, FLOAT topLeftX, FLOAT topLeftY, FLOAT minDepth, FLOAT maxDepth)
{
	D3D12_VIEWPORT viewport{};
	viewport.Width = width;
	viewport.Height = height;
	viewport.TopLeftX = topLeftX;
	viewport.TopLeftY = topLeftY;
	viewport.MinDepth = minDepth;
	viewport.MaxDepth = maxDepth;
	return viewport;
}

D3D12_RECT NRenderer::CreateScissorRect(FLOAT left, FLOAT right, FLOAT top, FLOAT bottom)
{
	D3D12_RECT scissorRect{};
	scissorRect.left = LONG(left);
	scissorRect.right = LONG(right);
	scissorRect.top = LONG(top);
	scissorRect.bottom = LONG(bottom);
	return scissorRect;
}

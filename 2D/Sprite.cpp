#include "Sprite.h"
#include "StringManager.h"
#include "TextureManager.h"
#include <cmath>
#include <numbers>
#include <cassert>
#include "D3D12Lib.h"

using namespace Microsoft::WRL;

ID3D12Device* Sprite::sDevice_ = nullptr;
ID3D12GraphicsCommandList* Sprite::sCommandList_;
UINT Sprite::sDescriptorHandleIncrementSize_;
Matrix4x4 Sprite::sMatProjection_;

ComPtr<ID3D12RootSignature> Sprite::sRootSignature_;
ComPtr<ID3D12PipelineState> Sprite::gPipelineState_;

Sprite* Sprite::Create(uint32_t textureHandle, Vector2 position, Vector4 color, Vector2 anchorpoint, bool isFlipX, bool isFlipY) {
	// 仮のサイズを追加
	Vector2 size = { 100.0f,100.0f };
	// テクスチャの設定
	{
		const D3D12_RESOURCE_DESC& rDesc = TextureManager::GetInstance()->GetResourceDesc(textureHandle);
		size = { float(rDesc.Width),float(rDesc.Height) };
	}


	Sprite* sprite =
		new Sprite(textureHandle, position, size, color, anchorpoint, isFlipX, isFlipY);
	if (sprite == nullptr) {
		return nullptr;
	}

	if (!sprite->Initialize()) {
		delete sprite;
		assert(0);
		return nullptr;
	}

	return sprite;
}

void Sprite::StaticInitialize(ID3D12Device* device, int window_width, int window_height) {
	// nullptrチェック
	assert(device);
	sDevice_ = device;
	HRESULT result = S_FALSE;

	// デスクリプタサイズを取得
	sDescriptorHandleIncrementSize_ =
		sDevice_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	// Shaderをコンパイルする
	ComPtr<IDxcBlob> vertexShaderBlob;	// 頂点シェーダオブジェクト
	ComPtr<IDxcBlob> pixelShaderBlob;	// ピクセルシェーダオブジェクト
	ComPtr<ID3DBlob> errorBlob;			// エラーオブジェクト

	// 頂点シェーダの読み込みとコンパイル
	vertexShaderBlob = D3D12Lib::GetInstance()->CompileShader(L"resources/shaders/Sprite.VS.hlsl",
		L"vs_6_0");
	assert(vertexShaderBlob != nullptr);

	// ピクセルシェーダの読み込みとコンパイル
	pixelShaderBlob = D3D12Lib::GetInstance()->CompileShader(L"resources/shaders/Sprite.PS.hlsl",
		L"ps_6_0");
	assert(pixelShaderBlob != nullptr);

	// InputLayout
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{	// xy
			"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0},
		{	// uv
			"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0},
	};

	// グラフィックスパイプライン
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gPipeline{};
	gPipeline.VS = D3D12Lib::ShaderByteCode(vertexShaderBlob.Get());		// VertexShader
	gPipeline.PS = D3D12Lib::ShaderByteCode(pixelShaderBlob.Get());		// PixelShader

	// サンプルマスクの設定
	gPipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

	// ラスタライザステートの設定
	D3D12_RASTERIZER_DESC rasterizerDesc{};
	// 裏面（時計回り）を表示しない
	rasterizerDesc.CullMode = D3D12_CULL_MODE_NONE;
	// 三角形の中を塗りつぶす
	rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;
	rasterizerDesc.DepthClipEnable = true;

	gPipeline.RasterizerState = rasterizerDesc;		// ラスタライザ

	// デプスステンシルステートの設定
	D3D12_DEPTH_STENCIL_DESC dsDesc{};
	dsDesc.DepthEnable = true;
	dsDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
	dsDesc.StencilReadMask = D3D12_DEFAULT_STENCIL_READ_MASK;
	dsDesc.StencilWriteMask = D3D12_DEFAULT_STENCIL_WRITE_MASK;

	gPipeline.DepthStencilState = dsDesc;

	const D3D12_DEPTH_STENCILOP_DESC defaultStencilOp =
	{ D3D12_STENCIL_OP_KEEP,D3D12_STENCIL_OP_KEEP,D3D12_STENCIL_OP_KEEP,D3D12_COMPARISON_FUNC_ALWAYS };

	dsDesc.FrontFace = defaultStencilOp;
	dsDesc.BackFace = defaultStencilOp;

	//dsDesc.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;

	// 深度バッファのフォーマット
	gPipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	// 頂点レイアウトの設定
	gPipeline.InputLayout.pInputElementDescs = inputLayout;
	gPipeline.InputLayout.NumElements = _countof(inputLayout);

	// 利用するトポロジ（形状）のタイプ。三角形
	gPipeline.PrimitiveTopologyType =
		D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	// 書き込むRTVの情報
	gPipeline.NumRenderTargets = 1;
	gPipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	gPipeline.SampleDesc.Count = 1;

	// デスクリプタレンジの作成
	D3D12_DESCRIPTOR_RANGE descRangeSRV[1];
	descRangeSRV[0] = D3D12Lib::Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);	// t0レジスタ

	// ルートパラメータの作成。複数設定できるので配列。
	D3D12_ROOT_PARAMETER rootParameters[2] = {};
	rootParameters[0] = D3D12Lib::InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootParameters[1] = D3D12Lib::InitAsDescriptorTable(1, descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);
	
	/// スタティックサンプラーの設定
	D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};
	staticSamplers[0] = D3D12Lib::SetSamplerDesc(0, D3D12_FILTER_MIN_MAG_MIP_LINEAR);
	staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;	// 0~1の範囲外をリピート
	staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;

	// RootSignature作成
	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};
	descriptionRootSignature.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	descriptionRootSignature.pParameters = rootParameters;	// ルートパラメータ配列へのポインタ
	descriptionRootSignature.NumParameters = _countof(rootParameters);


	descriptionRootSignature.pStaticSamplers = staticSamplers;	
	descriptionRootSignature.NumStaticSamplers = _countof(staticSamplers);

	// シリアライズしてバイナリにする
	ComPtr<ID3DBlob> signatureBlob;
	result = D3D12SerializeRootSignature(&descriptionRootSignature, D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob);
	if (FAILED(result)) {
		Log(reinterpret_cast<char*>(errorBlob->GetBufferPointer()));
		assert(false);
	}
	// ルートシグネチャの生成
	result = sDevice_->CreateRootSignature(0, signatureBlob->GetBufferPointer(),
		signatureBlob->GetBufferSize(), IID_PPV_ARGS(&sRootSignature_));
	assert(SUCCEEDED(result));

	gPipeline.pRootSignature = sRootSignature_.Get();	// ルートシグネチャ


	// BlendStateの設定
	D3D12_BLEND_DESC blendDesc{};
	// すべての色要素を書き込む
	blendDesc.RenderTarget[0].RenderTargetWriteMask =
		D3D12_COLOR_WRITE_ENABLE_ALL;

	gPipeline.BlendState = blendDesc;					// ブレンド
	

	// グラフィックスパイプラインの生成
	result = sDevice_->CreateGraphicsPipelineState(&gPipeline, IID_PPV_ARGS(&gPipelineState_));
	assert(SUCCEEDED(result));

	// 射影行列
	sMatProjection_ = MakeOrthographicMatrix(0.0f, 0.0f, (float)window_width, (float)window_height, 0.0f, 1.0f);

}

Sprite::Sprite(uint32_t textureHandle, Vector2 position, Vector2 size, Vector4 color, Vector2 anchorpoint, bool isFlipX, bool isFlipY)
{
	position_ = position;
	size_ = size;
	anchorpoint_ = anchorpoint;
	matWorld_ = MakeIdentity4x4();
	color_ = color;
	textureHandle_ = textureHandle;
	isFlipX_ = isFlipX;
	isFlipY_ = isFlipY;
	texSize_ = size;
}

bool Sprite::Initialize() {
	// デバイスのnullptrチェック
	assert(sDevice_);

	HRESULT result = S_FALSE;

	resourceDesc_ = TextureManager::GetInstance()->GetResourceDesc(textureHandle_);

	{
		// ヒーププロパティ
		D3D12_HEAP_PROPERTIES heapProps = D3D12Lib::SetHeapProperties(D3D12_HEAP_TYPE_UPLOAD);
		// リソース設定
		D3D12_RESOURCE_DESC resourceDesc = 
			D3D12Lib::SetResourceDesc(sizeof(VertexData) * kVertNum);

		// 頂点バッファ生成
		result = sDevice_->CreateCommittedResource(
			&heapProps, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr, IID_PPV_ARGS(&vertBuff_));
		assert(SUCCEEDED(result));

		// 頂点バッファマッピング
		vertBuff_->Map(0, nullptr, reinterpret_cast<void**>(&vertData_));
		assert(SUCCEEDED(result));
	}
	// 頂点バッファへの転送
	TransferVertices();

	vbView_.BufferLocation = vertBuff_->GetGPUVirtualAddress();
	vbView_.SizeInBytes = sizeof(VertexData) * kVertNum;
	vbView_.StrideInBytes = sizeof(VertexData);

	{
		// ヒーププロパティ
		D3D12_HEAP_PROPERTIES heapProps = D3D12Lib::SetHeapProperties(D3D12_HEAP_TYPE_UPLOAD);
		// リソース設定
		D3D12_RESOURCE_DESC resourceDesc = D3D12Lib::SetResourceDesc((sizeof(ConstBufferData) + 0xff) & ~0xff);
		// 定数バッファ生成
		result = sDevice_->CreateCommittedResource(
			&heapProps, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr, IID_PPV_ARGS(&constBuff_));
		assert(SUCCEEDED(result));
		// 定数バッファマッピング
		result = constBuff_->Map(0, nullptr, (void**)&constData_);
		assert(SUCCEEDED(result));

	}

	return true;
}

void Sprite::SetAnchorPoint(const Vector2& anchorPoint)
{
	anchorpoint_ = anchorPoint;

	// 頂点バッファへ転送
	TransferVertices();
}

void Sprite::TransferVertices()
{
	//HRESULT result = S_FALSE;

	// 4頂点
	enum { LB, LT, RB, RT };

	// 左右上下
	float left = (0.0f - anchorpoint_.x) * size_.x;
	float right = (1.0f - anchorpoint_.x) * size_.x;
	float top = (0.0f - anchorpoint_.y) * size_.y;
	float bottom = (1.0f - anchorpoint_.y) * size_.y;

	if (isFlipX_) {	// 左右反転
		left = -left;
		right = -right;
	}
	if (isFlipY_) {	// 上下反転
		top = -top;
		bottom = -bottom;
	}

	// 頂点情報
	VertexData vertices[kVertNum];

	vertices[LB].position = { left,bottom,0.0f };	// 左下
	vertices[LT].position = { left,top,0.0f };		// 左上
	vertices[RB].position = { right,bottom,0.0f };	// 右下
	vertices[RT].position = { right,top,0.0f };		// 右上

	// テクスチャ情報取得
	{
		float tex_left = texBase_.x / resourceDesc_.Width;
		float tex_right = (texBase_.x + texSize_.x) / resourceDesc_.Width;
		float tex_top = texBase_.y / resourceDesc_.Height;
		float tex_bottom = (texBase_.y + texSize_.y) / resourceDesc_.Height;

		vertices[LB].texcoord = { tex_left, tex_bottom };  // 左下
		vertices[LT].texcoord = { tex_left, tex_top };     // 左上
		vertices[RB].texcoord = { tex_right, tex_bottom }; // 右下
		vertices[RT].texcoord = { tex_right, tex_top };    // 右上
	}

	memcpy(vertData_, vertices, sizeof(vertices));
}

void Sprite::Draw() {
	// ワールド行列の更新
	matWorld_ = MakeIdentity4x4();
	matWorld_ = Multiply(matWorld_, MakeRotateZMatrix(rotation_));
	matWorld_ = Multiply(matWorld_, MakeTranslateMatrix(Vector3(position_.x, position_.y, 0)));
	
	// 定数バッファにデータ転送
	constData_->color = color_;
	constData_->mat = Multiply(matWorld_, sMatProjection_);

	// 頂点バッファの設定
	sCommandList_->IASetVertexBuffers(0, 1, &vbView_);

	// マテリアルCBufferの場所を設定
	sCommandList_->SetGraphicsRootConstantBufferView(0, constBuff_->GetGPUVirtualAddress());
	// シェーダリソースビューをセット
	TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(sCommandList_, 1, textureHandle_);

	// 描画（仮）
	sCommandList_->DrawInstanced(kVertNum, 1, 0, 0);

}

void Sprite::SetTextureHandle(uint32_t textureHandle)
{
	textureHandle_ = textureHandle;
	resourceDesc_ = TextureManager::GetInstance()->GetResourceDesc(textureHandle_);
}

void Sprite::SetPosition(const Vector2& position)
{
	position_ = position;

	// データ転送
	TransferVertices();
}

void Sprite::SetRotation(float rotation)
{
	rotation_ = rotation;

	// データ転送
	TransferVertices();
}

void Sprite::PreDraw(ID3D12GraphicsCommandList* commandList) {
	assert(Sprite::sCommandList_ == nullptr);

	sCommandList_ = commandList;
	// パイプラインの設定
	sCommandList_->SetPipelineState(gPipelineState_.Get());
	// ルートシグネチャの設定
	sCommandList_->SetGraphicsRootSignature(sRootSignature_.Get());

	// 形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えておけば良い
	sCommandList_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

}

void Sprite::PostDraw() {
	// コマンドリストを解除
	Sprite::sCommandList_ = nullptr;

}

#include "Sprite.h"
#include "StringManager.h"
#include "TextureManager.h"
#include <cassert>

using namespace Microsoft::WRL;

ID3D12Device* Sprite::sDevice_ = nullptr;
ID3D12GraphicsCommandList* Sprite::sCommandList_;

ComPtr<ID3D12RootSignature> Sprite::sRootSignature_;
ComPtr<IDxcUtils> Sprite::dxcUtils_;
ComPtr<IDxcCompiler3> Sprite::dxcCompiler_;
ComPtr<IDxcIncludeHandler> Sprite::includeHandler_;
ComPtr<ID3D12PipelineState> Sprite::gPipelineState_;

Sprite* Sprite::Create() {

	Sprite* sprite = new Sprite();
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

//void Sprite::StaticInitialize(ID3D12Device* device) {
//	// nullptrチェック
//	assert(device);
//	sDevice_ = device;
//	HRESULT result = S_FALSE;
//
//	// Dxc
//	InitializeDXC();
//	
//	// Shaderをコンパイルする
//	ComPtr<IDxcBlob> vertexShaderBlob;
//	ComPtr<IDxcBlob> pixelShaderBlob;
//
//	vertexShaderBlob = CompileShader(L"Object3D.VS.hlsl",
//		L"vs_6_0", dxcUtils_.Get(), dxcCompiler_.Get(), includeHandler_.Get());
//	assert(vertexShaderBlob != nullptr);
//
//	pixelShaderBlob = CompileShader(L"Object3D.PS.hlsl",
//		L"ps_6_0", dxcUtils_.Get(), dxcCompiler_.Get(), includeHandler_.Get());
//	assert(pixelShaderBlob != nullptr);
//
//	// RootSignature作成
//	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};
//	descriptionRootSignature.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
//
//	//// DescriptorRangeの作成
//	//D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
//	//descriptorRange[0].BaseShaderRegister = 0;	// 0から始まる
//	//descriptorRange[0].NumDescriptors = 1;		// 数は1つ
//	//descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;	// SRVを使う
//	//descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;	// Offsetを自動計算
//
//	// RootParameter作成。複数設定できるので配列。今回は結果1つだけなので長さ1の配列
//	D3D12_ROOT_PARAMETER rootParameters[2] = {};
//	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	// CBVを使う
//	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;	// PixelShaderで使う
//	rootParameters[0].Descriptor.ShaderRegister = 0;	// レジスタ番号0とバインド
//
//	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
//	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
//	rootParameters[1].Descriptor.ShaderRegister = 0;
//
//	//rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	// DescriptorTableを指定
//	//rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;	// PixelShaderで使う
//	//rootParameters[2].DescriptorTable.pDescriptorRanges = descriptorRange;	// Tableの中身の配列を指定
//	//rootParameters[2].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange);	// Tableで利用する数
//	
//	descriptionRootSignature.pParameters = rootParameters;	// ルートパラメータ配列へのポインタ
//	descriptionRootSignature.NumParameters = _countof(rootParameters);
//
//	//D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};
//	//staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;	// バイリニアフィルタ
//	//staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
//	//staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
//	//staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
//	//staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
//	//staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX;
//	//staticSamplers[0].ShaderRegister = 0;
//	//staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
//	//descriptionRootSignature.pStaticSamplers = staticSamplers;
//	//descriptionRootSignature.NumParameters = _countof(staticSamplers);
//
//	// シリアライズしてバイナリにする
//	ComPtr<ID3DBlob> signatureBlob;
//	ComPtr<ID3DBlob> errorBlob;
//	result = D3D12SerializeRootSignature(&descriptionRootSignature, D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob);
//	if (FAILED(result)) {
//		Log(reinterpret_cast<char*>(errorBlob->GetBufferPointer()));
//		assert(false);
//	}
//
//	// バイナリを元に生成
//	result = sDevice_->CreateRootSignature(0, signatureBlob->GetBufferPointer(),
//		signatureBlob->GetBufferSize(), IID_PPV_ARGS(&sRootSignature_));
//	assert(SUCCEEDED(result));
//
//	// InputLayout
//	D3D12_INPUT_ELEMENT_DESC inputElementDescs[2] = {};
//	inputElementDescs[0].SemanticName = "POSITION";
//	inputElementDescs[0].SemanticIndex = 0;
//	inputElementDescs[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
//	inputElementDescs[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
//	inputElementDescs[0].InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
//
//	//inputElementDescs[1].SemanticName = "TEXCOORD";
//	//inputElementDescs[1].SemanticIndex = 0;
//	//inputElementDescs[1].Format = DXGI_FORMAT_R32G32_FLOAT;
//	//inputElementDescs[1].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
//	//inputElementDescs[1].InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
//
//	//inputElementDescs[2].SemanticName = "NORMAL";
//	//inputElementDescs[2].SemanticIndex = 0;
//	//inputElementDescs[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
//	//inputElementDescs[2].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
//
//	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
//	inputLayoutDesc.pInputElementDescs = inputElementDescs;
//	inputLayoutDesc.NumElements = _countof(inputElementDescs);
//
//
//	// BlendStateの設定
//	D3D12_BLEND_DESC blendDesc{};
//	// すべての色要素を書き込む
//	blendDesc.RenderTarget[0].RenderTargetWriteMask =
//		D3D12_COLOR_WRITE_ENABLE_ALL;
//
//	// RasiterzerStateの設定
//	D3D12_RASTERIZER_DESC rasterizerDesc{};
//	// 裏面（時計回り）を表示しない
//	rasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
//	// 三角形の中を塗りつぶす
//	rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;
//
//	// PSOを作成
//	D3D12_GRAPHICS_PIPELINE_STATE_DESC gPipelineStateDesc{};
//	gPipelineStateDesc.pRootSignature = sRootSignature_.Get();
//	gPipelineStateDesc.InputLayout = inputLayoutDesc;
//	gPipelineStateDesc.VS = Sprite::ShaderByteCode(vertexShaderBlob.Get());		// VertexShader
//	gPipelineStateDesc.PS = Sprite::ShaderByteCode(pixelShaderBlob.Get());		// PixelShader
//	gPipelineStateDesc.BlendState = blendDesc;	// BlendState
//	gPipelineStateDesc.RasterizerState = rasterizerDesc;	// RasterizerState
//	// 書き込むRTVの情報
//	gPipelineStateDesc.NumRenderTargets = 1;
//	gPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
//	// 利用するトポロジ（形状）のタイプ。三角形
//	gPipelineStateDesc.PrimitiveTopologyType =
//		D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
//	// どのように画面に色を打ち込むかの設定（気にしなくてよい）
//	gPipelineStateDesc.SampleDesc.Count = 1;
//	gPipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
//
//	// グラフィックスパイプラインの生成
//	result = sDevice_->CreateGraphicsPipelineState(&gPipelineStateDesc, IID_PPV_ARGS(&gPipelineState_));
//	assert(SUCCEEDED(result));
//
//}

void Sprite::StaticInitialize(ID3D12Device* device) {
	// nullptrチェック
	assert(device);
	sDevice_ = device;
	HRESULT result = S_FALSE;

	// Dxc
	InitializeDXC();

	// Shaderをコンパイルする
	ComPtr<IDxcBlob> vertexShaderBlob;
	ComPtr<IDxcBlob> pixelShaderBlob;

	vertexShaderBlob = CompileShader(L"Object3D.VS.hlsl",
		L"vs_6_0", dxcUtils_.Get(), dxcCompiler_.Get(), includeHandler_.Get());
	assert(vertexShaderBlob != nullptr);

	pixelShaderBlob = CompileShader(L"Object3D.PS.hlsl",
		L"ps_6_0", dxcUtils_.Get(), dxcCompiler_.Get(), includeHandler_.Get());
	assert(pixelShaderBlob != nullptr);

	// RootSignature作成
	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};
	descriptionRootSignature.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	//// DescriptorRangeの作成
	//D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
	//descriptorRange[0].BaseShaderRegister = 0;	// 0から始まる
	//descriptorRange[0].NumDescriptors = 1;		// 数は1つ
	//descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;	// SRVを使う
	//descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;	// Offsetを自動計算


	// RootParameter作成。複数設定できるので配列。
	D3D12_ROOT_PARAMETER rootParameters[2] = {};
	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	// CBVを使う
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;	// PixelShaderで使う
	rootParameters[0].Descriptor.ShaderRegister = 0;	// レジスタ番号0とバインド

	// 1
	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
	rootParameters[1].Descriptor.ShaderRegister = 0;

	//// 2
	//rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	// DescriptorTableを使う
	//rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;	// PixelShaderで使う
	//rootParameters[2].DescriptorTable.pDescriptorRanges = descriptorRange;	// Tableの中身の配列を指定
	//rootParameters[2].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange);	// Tableで利用する数

	///// Samplerの設定
	//D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};
	//staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;	// バイリニアフィルタ
	//staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;	// 0~1の範囲外をリピート
	//staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	//staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	//staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;	// 比較しない
	//staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX;	// ありったけのMipmapを使う
	//staticSamplers[0].ShaderRegister = 0;	// レジスタ番号0を使う
	//staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;	// PixelShaderで使う
	//descriptionRootSignature.pStaticSamplers = staticSamplers;
	//descriptionRootSignature.NumParameters = _countof(staticSamplers);

	descriptionRootSignature.pParameters = rootParameters;	// ルートパラメータ配列へのポインタ
	descriptionRootSignature.NumParameters = _countof(rootParameters);

	// シリアライズしてバイナリにする
	ComPtr<ID3DBlob> signatureBlob;
	ComPtr<ID3DBlob> errorBlob;
	result = D3D12SerializeRootSignature(&descriptionRootSignature, D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob);
	if (FAILED(result)) {
		Log(reinterpret_cast<char*>(errorBlob->GetBufferPointer()));
		assert(false);
	}

	// バイナリを元に生成
	result = sDevice_->CreateRootSignature(0, signatureBlob->GetBufferPointer(),
		signatureBlob->GetBufferSize(), IID_PPV_ARGS(&sRootSignature_));
	assert(SUCCEEDED(result));

	// InputLayout
	D3D12_INPUT_ELEMENT_DESC inputElementDescs[2] = {};
	inputElementDescs[0].SemanticName = "POSITION";
	inputElementDescs[0].SemanticIndex = 0;
	inputElementDescs[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	inputElementDescs[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	inputElementDescs[1].SemanticName = "TEXCOORD";
	inputElementDescs[1].SemanticIndex = 0;
	inputElementDescs[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	inputElementDescs[1].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	//inputElementDescs[2].SemanticName = "NORMAL";
	//inputElementDescs[2].SemanticIndex = 0;
	//inputElementDescs[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	//inputElementDescs[2].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
	inputLayoutDesc.pInputElementDescs = inputElementDescs;
	inputLayoutDesc.NumElements = _countof(inputElementDescs);


	// BlendStateの設定
	D3D12_BLEND_DESC blendDesc{};
	// すべての色要素を書き込む
	blendDesc.RenderTarget[0].RenderTargetWriteMask =
		D3D12_COLOR_WRITE_ENABLE_ALL;

	// RasiterzerStateの設定
	D3D12_RASTERIZER_DESC rasterizerDesc{};
	// 裏面（時計回り）を表示しない
	rasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
	// 三角形の中を塗りつぶす
	rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;

	// PSOを作成
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gPipelineStateDesc{};
	gPipelineStateDesc.pRootSignature = sRootSignature_.Get();
	gPipelineStateDesc.InputLayout = inputLayoutDesc;
	gPipelineStateDesc.VS = Sprite::ShaderByteCode(vertexShaderBlob.Get());		// VertexShader
	gPipelineStateDesc.PS = Sprite::ShaderByteCode(pixelShaderBlob.Get());		// PixelShader
	gPipelineStateDesc.BlendState = blendDesc;	// BlendState
	gPipelineStateDesc.RasterizerState = rasterizerDesc;	// RasterizerState
	// 書き込むRTVの情報
	gPipelineStateDesc.NumRenderTargets = 1;
	gPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	// 利用するトポロジ（形状）のタイプ。三角形
	gPipelineStateDesc.PrimitiveTopologyType =
		D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	// どのように画面に色を打ち込むかの設定（気にしなくてよい）
	gPipelineStateDesc.SampleDesc.Count = 1;
	gPipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

	// グラフィックスパイプラインの生成
	result = sDevice_->CreateGraphicsPipelineState(&gPipelineStateDesc, IID_PPV_ARGS(&gPipelineState_));
	assert(SUCCEEDED(result));

}

void Sprite::PreDraw(ID3D12GraphicsCommandList* commandList) {
	assert(Sprite::sCommandList_ == nullptr);

	sCommandList_ = commandList;
	// ルートシグネチャの設定
	sCommandList_->SetGraphicsRootSignature(sRootSignature_.Get());
	// パイプラインの設定
	sCommandList_->SetPipelineState(gPipelineState_.Get());
	
	// 形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えておけば良い
	sCommandList_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

}

void Sprite::PostDraw() {
	// コマンドリストを解除
	Sprite::sCommandList_ = nullptr;

}

void Sprite::Draw() {

	wvpData->WVP = wvpMatrix_;

	sCommandList_->IASetVertexBuffers(0, 1, &vertBufferView_);
	// マテリアルCBufferの場所を設定
	sCommandList_->SetGraphicsRootConstantBufferView(0, constBuff_->GetGPUVirtualAddress());

	// wvp用のCBufferの場所を設定
	sCommandList_->SetGraphicsRootConstantBufferView(1, wvpResoure_->GetGPUVirtualAddress());

	// シェーダリソースビューをセット
	//TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(sCommandList_, 1, textureHandle_);

	// 描画（仮）
	sCommandList_->DrawInstanced(3, 1, 0, 0);

	// SRVのDescriptorTableの先頭を設定。2はrootParameter[2]である。
	//sCommandList_->SetGraphicsRootDescriptorTable(2,)
}

bool Sprite::Initialize() {
	// デバイスのnullptrチェック
	assert(sDevice_);

	HRESULT result = S_FALSE;

	{
		// 頂点リソース用のヒープの設定
		D3D12_HEAP_PROPERTIES uploadHeapProps{};
		uploadHeapProps.Type = D3D12_HEAP_TYPE_UPLOAD;

		vertBuff_ = CreateBufferResoruce(sizeof(VertexData) * 3);

		vertBuff_->Map(0, nullptr, reinterpret_cast<void**>(&vertData_));
		// 左下
		vertData_[0].position = { -0.5f,-0.5f,0.0f,1.0f };
		vertData_[0].texcoord = { 0.0f,1.0f };

		// 上
		vertData_[1].position = { 0.0f,0.5f,0.0f,1.0f };
		vertData_[1].texcoord = { 0.5f,0.0f };

		// 右下
		vertData_[2].position = { 0.5f,-0.5f,0.0f,1.0f };
		vertData_[2].texcoord = { 1.0f,1.0f };
	}

	vertBufferView_.BufferLocation = vertBuff_->GetGPUVirtualAddress();
	vertBufferView_.SizeInBytes = sizeof(VertexData) * 3;
	vertBufferView_.StrideInBytes = sizeof(VertexData);

	{
		// マテリアル用のリソースを作る。今回はcolor1つ分のサイズを用意する
		constBuff_ = CreateBufferResoruce(sizeof(Vector4));

	}

	result = constBuff_->Map(0, nullptr, (void**)&constData_);
	assert(SUCCEEDED(result));
	*constData_ = Vector4(1.0f, 1.0f, 0.0f, 1.0f);

	// WVP用のリソースのサイズを用意
	wvpResoure_ = CreateBufferResoruce(sizeof(TransformationMatrix));
	// 書き込むためのアドレスを取得
	wvpResoure_->Map(0, nullptr, reinterpret_cast<void**>(&wvpData));
	// 単位行列を書き込んでおく
	wvpData->WVP = MakeIdentity4x4();

	return true;
}


void Sprite::InitializeDXC() {
	HRESULT result = S_FALSE;
	result = DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&dxcUtils_));

	assert(SUCCEEDED(result));
	result = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&dxcCompiler_));
	assert(SUCCEEDED(result));
	
	result = dxcUtils_->CreateDefaultIncludeHandler(includeHandler_.GetAddressOf());
	assert(SUCCEEDED(result));
}

D3D12_SHADER_BYTECODE Sprite::ShaderByteCode(IDxcBlob* blob) {

	D3D12_SHADER_BYTECODE shaderByte = { blob->GetBufferPointer(),blob->GetBufferSize() };
	return shaderByte;
}

ComPtr<ID3D12Resource> Sprite::CreateBufferResoruce(size_t sizeInBytes) {
	// nullptrチェック
	assert(sDevice_);

	HRESULT result = S_FALSE;
	// 頂点リソース用のヒープの設定
	D3D12_HEAP_PROPERTIES uploadHeapProperties{};
	uploadHeapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;	// UploadHeapを使う
	// 頂点リソースの設定
	D3D12_RESOURCE_DESC resourceDesc{};
	// バッファリソース。テクスチャの場合はまた別の設定をする
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resourceDesc.Width = sizeInBytes;	// リソースのサイズ
	// バッファの場合はこれらは1にする決まり
	resourceDesc.Height = 1;
	resourceDesc.DepthOrArraySize = 1;
	resourceDesc.MipLevels = 1;
	resourceDesc.SampleDesc.Count = 1;
	// バッファの場合はこれにする決まり
	resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	// 実際に頂点リソースを作る
	ID3D12Resource* resultResource = nullptr;
	result = sDevice_->CreateCommittedResource(&uploadHeapProperties, D3D12_HEAP_FLAG_NONE,
		&resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&resultResource));
	assert(SUCCEEDED(result));

	return resultResource;
}

D3D12_RESOURCE_DESC Sprite::SetResourceDesc(size_t size)
{
	D3D12_RESOURCE_DESC resourceDesc{};
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resourceDesc.Width = size;	// リソースサイズ
	// バッファの場合はこれらは1にする決まり
	resourceDesc.Height = 1;
	resourceDesc.DepthOrArraySize = 1;
	resourceDesc.MipLevels = 1;
	resourceDesc.SampleDesc.Count = 1;
	// バッファの場合これにする決まり
	resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	return resourceDesc;
}

IDxcBlob* Sprite::CompileShader(
	// CompilerするShaderファイルへのパス
	const std::wstring& filePath,
	// Compilerに使用するProfile
	const wchar_t* profile,
	// 初期化で生成したものを3つ
	IDxcUtils* dxcUtils,
	IDxcCompiler3* dxcCompiler,
	IDxcIncludeHandler* includeHandler) {

	// ここの中身をこの後書く
		// 1.hlslファイルを読む
		// これからシェーダーをコンパイルする旨をログに出す
	Log(ConvertString(std::format(L"Begin CompileShader, path:{}, profile:{}\n", filePath, profile)));
	// hlslファイルを読む
	IDxcBlobEncoding* shaderSource = nullptr;
	HRESULT hr = dxcUtils->LoadFile(filePath.c_str(), nullptr, &shaderSource);
	// 読めなかったら止める
	assert(SUCCEEDED(hr));
	// 読み込んだファイルの内容を設定する
	DxcBuffer shaderSourceBuffer;
	shaderSourceBuffer.Ptr = shaderSource->GetBufferPointer();
	shaderSourceBuffer.Size = shaderSource->GetBufferSize();
	shaderSourceBuffer.Encoding = DXC_CP_UTF8;// UTF8の文字コードであることを通知
	// 2.Compileする
	LPCWSTR arguments[] = {
		filePath.c_str(),	// コンパイル対象のhlslファイル名
		L"-E", L"main",	// エントリーポイントの指定。基本的にmain以外にはしない
		L"-T", profile,	// ShaderProfileの設定
		L"-Zi", L"-Qembed_debug",	// デバッグ用の情報を埋め込む
		L"-Od",	// 最適化を外しておく
		L"-Zpr",	// メモリレイアウトは行優先
	};
	// 実際にShaderをコンパイルする
	IDxcResult* shaderResult = nullptr;
	hr = dxcCompiler->Compile(
		&shaderSourceBuffer,	// 読み込んだファイル
		arguments,				// コンパイルオプション
		_countof(arguments),	// コンパイルオプションの数
		includeHandler,			// includeが含まれた諸々
		IID_PPV_ARGS(&shaderResult)	// コンパイル結果
	);
	// コンパイルエラーではなくdxcが起動できないなど致命的な状況
	assert(SUCCEEDED(hr));
	// 3.警告・エラーを確認する
	// 警告・エラーが出てたらログに出して止める
	IDxcBlobUtf8* shaderError = nullptr;
	shaderResult->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&shaderError), nullptr);
	if (shaderError != nullptr && shaderError->GetStringLength() != 0) {
		Log(shaderError->GetStringPointer());
		// 警告・エラーダメゼッタイ
		assert(false);
	}
	// 4.Compile結果を受け取って返す
	IDxcBlob* shaderBlob = nullptr;
	hr = shaderResult->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&shaderBlob), nullptr);
	assert(SUCCEEDED(hr));
	// 成功したログを出す
	Log(ConvertString(std::format(L"Compile Succeeded, path:{}, profile:{}\n", filePath, profile)));
	// もう使わないリソースを解放
	shaderSource->Release();
	shaderResult->Release();
	// 実行用のバイナリを返却
	return shaderBlob;
}

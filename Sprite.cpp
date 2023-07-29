#include "Sprite.h"
#include "StringManager.h"
#include "TextureManager.h"
#include <cmath>
#include <numbers>
#include <cassert>

using namespace Microsoft::WRL;

ID3D12Device* Sprite::sDevice_ = nullptr;
ID3D12GraphicsCommandList* Sprite::sCommandList_;
UINT Sprite::sDescriptorHandleIncrementSize_;

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

void Sprite::StaticInitialize(ID3D12Device* device) {
	// nullptrチェック
	assert(device);
	sDevice_ = device;
	HRESULT result = S_FALSE;

	// デスクリプタサイズを取得
	sDescriptorHandleIncrementSize_ =
		sDevice_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	// Dxc
	InitializeDXC();

	// Shaderをコンパイルする
	ComPtr<IDxcBlob> vertexShaderBlob;	// 頂点シェーダオブジェクト
	ComPtr<IDxcBlob> pixelShaderBlob;	// ピクセルシェーダオブジェクト
	ComPtr<ID3DBlob> errorBlob;			// エラーオブジェクト

	// 頂点シェーダの読み込みとコンパイル
	vertexShaderBlob = CompileShader(L"Object3D.VS.hlsl",
		L"vs_6_0", dxcUtils_.Get(), dxcCompiler_.Get(), includeHandler_.Get());
	assert(vertexShaderBlob != nullptr);

	// ピクセルシェーダの読み込みとコンパイル
	pixelShaderBlob = CompileShader(L"Object3D.PS.hlsl",
		L"ps_6_0", dxcUtils_.Get(), dxcCompiler_.Get(), includeHandler_.Get());
	assert(pixelShaderBlob != nullptr);

	// RootSignature作成
	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};
	descriptionRootSignature.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	// DescriptorRangeの作成
	D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
	descriptorRange[0].BaseShaderRegister = 0;	// 0から始まる
	descriptorRange[0].NumDescriptors = 1;		// 数は1つ
	descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;	// SRVを使う
	descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;	// Offsetを自動計算


	// RootParameter作成。複数設定できるので配列。
	D3D12_ROOT_PARAMETER rootParameters[3] = {};
	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	// CBVを使う
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;	// PixelShaderで使う
	rootParameters[0].Descriptor.ShaderRegister = 0;	// レジスタ番号0とバインド

	// 1
	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
	rootParameters[1].Descriptor.ShaderRegister = 0;

	// 2
	rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	// DescriptorTableを使う
	rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;	// PixelShaderで使う
	rootParameters[2].DescriptorTable.pDescriptorRanges = descriptorRange;	// Tableの中身の配列を指定
	rootParameters[2].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange);	// Tableで利用する数

	descriptionRootSignature.pParameters = rootParameters;	// ルートパラメータ配列へのポインタ
	descriptionRootSignature.NumParameters = _countof(rootParameters);

	/// Samplerの設定
	D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};
	staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;	// バイリニアフィルタ
	staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;	// 0~1の範囲外をリピート
	staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;	// 比較しない
	staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX;	// ありったけのMipmapを使う
	staticSamplers[0].ShaderRegister = 0;	// レジスタ番号0を使う
	staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;	// PixelShaderで使う

	descriptionRootSignature.pStaticSamplers = staticSamplers;	
	descriptionRootSignature.NumStaticSamplers = _countof(staticSamplers);

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
	gPipelineStateDesc.SampleDesc.Count = _countof(staticSamplers);
	
	gPipelineStateDesc.VS = Sprite::ShaderByteCode(vertexShaderBlob.Get());		// VertexShader
	gPipelineStateDesc.PS = Sprite::ShaderByteCode(pixelShaderBlob.Get());		// PixelShader
	gPipelineStateDesc.InputLayout = inputLayoutDesc;			// インプットレイアウト
	gPipelineStateDesc.BlendState = blendDesc;					// ブレンド
	gPipelineStateDesc.RasterizerState = rasterizerDesc;		// ラスタライザ
	D3D12_DEPTH_STENCIL_DESC dsDesc{};
	dsDesc.DepthEnable = true;
	dsDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
	gPipelineStateDesc.DepthStencilState = dsDesc;
	gPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

	// 書き込むRTVの情報
	gPipelineStateDesc.NumRenderTargets = 1;
	gPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	// 利用するトポロジ（形状）のタイプ。三角形
	gPipelineStateDesc.PrimitiveTopologyType =
		D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	// どのように画面に色を打ち込むかの設定（気にしなくてよい）
	gPipelineStateDesc.SampleDesc.Count = 1;
	gPipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

	

	// シリアライズしてバイナリにする
	ComPtr<ID3DBlob> signatureBlob;
	result = D3D12SerializeRootSignature(&descriptionRootSignature, D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob);
	if (FAILED(result)) {
		Log(reinterpret_cast<char*>(errorBlob->GetBufferPointer()));
		assert(false);
	}
	// バイナリを元に生成
	result = sDevice_->CreateRootSignature(0, signatureBlob->GetBufferPointer(),
		signatureBlob->GetBufferSize(), IID_PPV_ARGS(&sRootSignature_));
	assert(SUCCEEDED(result));

	gPipelineStateDesc.pRootSignature = sRootSignature_.Get();	// ルートシグネチャ

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
	wvpSpriteData_->WVP = wvpSpriteMat_;
	wvpSphereData_->WVP = wvpSphereMatrix_;
	*constData_ = color_;

	// マテリアルCBufferの場所を設定
	sCommandList_->SetGraphicsRootConstantBufferView(0, constBuff_->GetGPUVirtualAddress());

	// シェーダリソースビューをセット
	TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(sCommandList_, 2, textureHandle_);

	// スプライト
	// VBVの設定
	sCommandList_->IASetVertexBuffers(0, 1, &vertSpriteBufferView_);
	// wvp用のCBufferの場所を設定
	sCommandList_->SetGraphicsRootConstantBufferView(1, wvpSpriteResource_->GetGPUVirtualAddress());

	if (IsSprite_) {
		sCommandList_->DrawInstanced(6, 1, 0, 0);
	}
	// 三角形
	// VBVの設定
	sCommandList_->IASetVertexBuffers(0, 1, &vertBufferView_);
	// wvp用のCBufferの場所を設定
	sCommandList_->SetGraphicsRootConstantBufferView(1, wvpResoure_->GetGPUVirtualAddress());
	// 描画（仮）
	if (IsTriangle_) {
		sCommandList_->DrawInstanced(kVertNum, 1, 0, 0);
	}
	// 球
	// VBVの設定
	sCommandList_->IASetVertexBuffers(0, 1, &vertSphereBufferView_);
	// wvp用のCBufferの場所を設定
	sCommandList_->SetGraphicsRootConstantBufferView(1, wvpSphereResource_->GetGPUVirtualAddress());
	// 描画（仮）
	if (IsSphere_) {
		sCommandList_->DrawInstanced(kVertexIndex, 1, 0, 0);
	}

}

bool Sprite::Initialize() {
	// デバイスのnullptrチェック
	assert(sDevice_);

	HRESULT result = S_FALSE;


	// WVP用のリソースのサイズを用意
	wvpResoure_ = CreateBufferResource(sizeof(TransformationMatrix));
	// 書き込むためのアドレスを取得
	wvpResoure_->Map(0, nullptr, reinterpret_cast<void**>(&wvpData));
	// 単位行列を書き込んでおく
	wvpData->WVP = MakeIdentity4x4();	

	// WVP用のリソースのサイズを用意
	wvpSpriteResource_ = CreateBufferResource(sizeof(TransformationMatrix));
	// 書き込むためのアドレスを取得
	wvpSpriteResource_->Map(0, nullptr, reinterpret_cast<void**>(&wvpSpriteData_));
	// 単位行列を書き込んでおく
	wvpSpriteData_->WVP = MakeIdentity4x4();

	// リソースのサイズ
	wvpSphereResource_ = CreateBufferResource(sizeof(TransformationMatrix));
	// アドレス取得
	wvpSphereResource_->Map(0, nullptr, reinterpret_cast<void**>(&wvpSphereData_));
	// 単位行列
	wvpSphereData_->WVP = MakeIdentity4x4();

	{
		// 三角形
		vertBuff_ = CreateBufferResource(sizeof(VertexData) * kVertNum);

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

		// 左下2
		vertData_[3].position = { -0.5f,-0.5f,0.5f,1.0f };
		vertData_[3].texcoord = { 0.0f,1.0f };

		// 上2
		vertData_[4].position = { 0.0f,0.0f,0.0f,1.0f };
		vertData_[4].texcoord = { 0.5f,0.0f };

		// 右下2
		vertData_[5].position = { 0.5f,-0.5f,-0.5f,1.0f };
		vertData_[5].texcoord = { 1.0f,1.0f };

	}

	vertBufferView_.BufferLocation = vertBuff_->GetGPUVirtualAddress();
	vertBufferView_.SizeInBytes = sizeof(VertexData) * kVertNum;
	vertBufferView_.StrideInBytes = sizeof(VertexData);


	{

		vertSpriteBuff_ = CreateBufferResource(sizeof(VertexData) * 6);

		vertSpriteBuff_->Map(0, nullptr, reinterpret_cast<void**>(&vertSpriteData_));

		// 1枚目の三角形
		vertSpriteData_[0].position = { 0.0f,360.0f,0.0f,1.0f };// 左下
		vertSpriteData_[0].texcoord = { 0.0f,1.0f };
		vertSpriteData_[1].position = { 0.0f,0.0f,0.0f,1.0f };// 左上
		vertSpriteData_[1].texcoord = { 0.0f,0.0f };
		vertSpriteData_[2].position = { 640.0f,360.0f,0.0f,1.0f };
		vertSpriteData_[2].texcoord = { 1.0f,1.0f };
		// 2枚目の三角形
		vertSpriteData_[3].position = { 0.0f,0.0f,0.0f,1.0f };
		vertSpriteData_[3].texcoord = { 0.0f,0.0f };
		vertSpriteData_[4].position = { 640.0f,0.0f,0.0f,1.0f };
		vertSpriteData_[4].texcoord = { 1.0f,0.0f };
		vertSpriteData_[5].position = { 640.0f,360.0f,0.0f,1.0f };
		vertSpriteData_[5].texcoord = { 1.0f,1.0f };

	}

	vertSpriteBufferView_.BufferLocation = vertSpriteBuff_->GetGPUVirtualAddress();
	vertSpriteBufferView_.SizeInBytes = sizeof(VertexData) * 6;
	vertSpriteBufferView_.StrideInBytes = sizeof(VertexData);

	{
		vertSphereBuff_ = CreateBufferResource(sizeof(VertexData) * kVertexIndex);

		vertSphereBuff_->Map(0, nullptr, reinterpret_cast<void**>(&vertSphereData_));

		// 経度分割１つ分の角度 φd
		const float kLonEvery = float(std::numbers::pi) * 2.0f / float(kSubdivision);
		// 緯度分割１つ分の角度 θd
		const float kLatEvery = float(std::numbers::pi) / float(kSubdivision);
		// 緯度の方向に分割
		for (uint32_t latIndex = 0; latIndex < kSubdivision; ++latIndex) {
			float lat = float(-std::numbers::pi) / 2.0f + kLatEvery * latIndex; // θ
			for (uint32_t lonIndex = 0; lonIndex < kSubdivision; ++lonIndex) {
				uint32_t index = (latIndex * kSubdivision + lonIndex) * 6;
				float lon = lonIndex * kLonEvery; // φ
				// 頂点にデータを入力する。基準点a 1
				vertSphereData_[index].position.x = std::cosf(lat) * std::cosf(lon);
				vertSphereData_[index].position.y = std::sinf(lat);
				vertSphereData_[index].position.z = std::cosf(lat) * std::sinf(lon);
				vertSphereData_[index].position.w = 1.0f;
				vertSphereData_[index].texcoord.x = float(lonIndex) / float(kSubdivision);
				vertSphereData_[index].texcoord.y = 1.0f - float(latIndex) / float(kSubdivision);

				// 頂点b 2
				vertSphereData_[index + 1].position.x = std::cosf(lat + kLatEvery) * std::cosf(lon);
				vertSphereData_[index + 1].position.y = std::sinf(lat + kLatEvery);
				vertSphereData_[index + 1].position.z = std::cosf(lat + kLatEvery) * std::sinf(lon);
				vertSphereData_[index + 1].position.w = 1.0f;
				vertSphereData_[index + 1].texcoord.x = float(lonIndex) / float(kSubdivision);
				vertSphereData_[index + 1].texcoord.y = 1.0f - float(latIndex) / float(kSubdivision);

				// 頂点c 3
				vertSphereData_[index + 2].position.x = std::cosf(lat) * std::cosf(lon + kLonEvery);
				vertSphereData_[index + 2].position.y = std::sinf(lat);
				vertSphereData_[index + 2].position.z = std::cosf(lat) * std::sinf(lon + kLonEvery);
				vertSphereData_[index + 2].position.w = 1.0f;
				vertSphereData_[index + 2].texcoord.x = float(lonIndex) / float(kSubdivision);
				vertSphereData_[index + 2].texcoord.y = 1.0f - float(latIndex) / float(kSubdivision);

				// 頂点c 4
				vertSphereData_[index + 3].position.x = std::cosf(lat) * std::cosf(lon + kLonEvery);
				vertSphereData_[index + 3].position.y = std::sinf(lat);
				vertSphereData_[index + 3].position.z = std::cosf(lat) * std::sinf(lon + kLonEvery);
				vertSphereData_[index + 3].position.w = 1.0f;
				vertSphereData_[index + 3].texcoord.x = float(lonIndex) / float(kSubdivision);
				vertSphereData_[index + 3].texcoord.y = 1.0f - float(latIndex) / float(kSubdivision);

				// 頂点b 5
				vertSphereData_[index + 4].position.x = std::cosf(lat + kLatEvery) * std::cosf(lon);
				vertSphereData_[index + 4].position.y = std::sinf(lat + kLatEvery);
				vertSphereData_[index + 4].position.z = std::cosf(lat + kLatEvery) * std::sinf(lon);
				vertSphereData_[index + 4].position.w = 1.0f;
				vertSphereData_[index + 4].texcoord.x = float(lonIndex) / float(kSubdivision);
				vertSphereData_[index + 4].texcoord.y = 1.0f - float(latIndex) / float(kSubdivision);

				// 頂点d 6
				vertSphereData_[index + 5].position.x = std::cosf(lat + kLatEvery) * std::cosf(lon + kLonEvery);
				vertSphereData_[index + 5].position.y = std::sinf(lat + kLatEvery);
				vertSphereData_[index + 5].position.z = std::cosf(lat + kLatEvery) * std::sinf(lon + kLonEvery);
				vertSphereData_[index + 5].position.w = 1.0f;
				vertSphereData_[index + 5].texcoord.x = float(lonIndex) / float(kSubdivision);
				vertSphereData_[index + 5].texcoord.y = 1.0f - float(latIndex) / float(kSubdivision);

			}
		}
	}

	vertSphereBufferView_.BufferLocation = vertSphereBuff_->GetGPUVirtualAddress();
	vertSphereBufferView_.SizeInBytes = sizeof(VertexData) * kVertexIndex;
	vertSphereBufferView_.StrideInBytes = sizeof(VertexData);


	{
		// マテリアル用のリソースを作る。今回はcolor1つ分のサイズを用意する
		constBuff_ = CreateBufferResource(sizeof(Vector4));

	}

	result = constBuff_->Map(0, nullptr, (void**)&constData_);
	assert(SUCCEEDED(result));
	*constData_ = Vector4(1.0f, 1.0f, 1.0f, 1.0f);

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

ComPtr<ID3D12Resource> Sprite::CreateBufferResource(size_t sizeInBytes) {
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

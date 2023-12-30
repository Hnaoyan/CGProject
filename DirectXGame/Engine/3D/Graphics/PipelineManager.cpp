#include "PipelineManager.h"
#include "DirectXCommon.h"
#include "D3D12Lib.h"
#include "StringManager.h"
#include "Shader.h"

#include <string>
#include <cassert>
#include <fstream>
#include <sstream>

#pragma comment(lib, "d3dcompiler.lib")

using namespace Microsoft::WRL;
using namespace Pipeline;

//std::array<
//	std::array<Microsoft::WRL::ComPtr<ID3D12PipelineState>, size_t(Pipeline::BlendMode::kCountOfBlendMode)>,
//	size_t(Pipeline::DrawType::kCountOfDrawer)> PipelineManager::sPipelineStates_;
Microsoft::WRL::ComPtr<ID3D12RootSignature> PipelineManager::sRootSignature_;
std::array<Microsoft::WRL::ComPtr<ID3D12PipelineState>,
	size_t(Pipeline::BlendMode::kCountOfBlendMode)> PipelineManager::sPipelineStates_;
void PipelineManager::CreatePipeline()
{
	HRESULT result = S_FALSE;
	ComPtr<IDxcBlob> vsBlob;
	ComPtr<IDxcBlob> psBlob;
	ComPtr<ID3DBlob> errorBlob;
	ComPtr<ID3DBlob> rootSigBlob;

	// 頂点シェーダの読み込みとコンパイル
	vsBlob = Shader::GetInstance()->Compile(L"ObjVS.hlsl", L"vs_6_0");
	assert(vsBlob != nullptr);

	// ピクセルシェーダの読み込みとコンパイル
	psBlob = Shader::GetInstance()->Compile(L"ObjPS.hlsl", L"ps_6_0");
	assert(psBlob != nullptr);

	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
	{
		SetInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT)
	},
	{
		SetInputLayout("NORMAL", DXGI_FORMAT_R32G32B32_FLOAT)
	},
	{
		SetInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT)
	},
	};

	// グラフィックスパイプライン
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
	gpipeline.VS = D3D12Lib::ShaderByteCode(vsBlob.Get());
	gpipeline.PS = D3D12Lib::ShaderByteCode(psBlob.Get());
	
	// サンプルマスク
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
	// ラスタライザステート
	D3D12_RASTERIZER_DESC rasterizer= SetRasterizerState(D3D12_FILL_MODE_SOLID, D3D12_CULL_MODE_BACK);
	gpipeline.RasterizerState = rasterizer;
	// デプスステンシルステート
	gpipeline.DepthStencilState.DepthEnable = true;
	gpipeline.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	//gpipeline.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
	gpipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;

	// 深度バッファのフォーマット
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	// 頂点レイアウト
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	// 図形の形状設定
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	gpipeline.NumRenderTargets = 1;
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	gpipeline.SampleDesc.Count = 1;

	// デスクリプタレンジ
	D3D12_DESCRIPTOR_RANGE descRangeSRV;
	descRangeSRV = D3D12Lib::Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);

	// ルートパラメータ
	D3D12_ROOT_PARAMETER rootparams[static_cast<int>(RootParameter::kCountOfParameter)];
	rootparams[static_cast<int>(RootParameter::kWorldTransform)] = D3D12Lib::InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_VERTEX);
	// View
	rootparams[static_cast<int>(RootParameter::kViewProjection)] = D3D12Lib::InitAsConstantBufferView(1, 0, D3D12_SHADER_VISIBILITY_ALL);
	// マテリアル
	rootparams[static_cast<int>(RootParameter::kMaterial)] = D3D12Lib::InitAsConstantBufferView(2, 0, D3D12_SHADER_VISIBILITY_PIXEL);
	// テクスチャ
	rootparams[static_cast<int>(RootParameter::kTexture)] = D3D12Lib::InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_PIXEL);
	// ライト
	rootparams[static_cast<int>(RootParameter::kLight)] = D3D12Lib::InitAsConstantBufferView(3, 0, D3D12_SHADER_VISIBILITY_PIXEL);

	// スタティックサンプラー
	D3D12_STATIC_SAMPLER_DESC samplerDesc[1];
	samplerDesc[0] = D3D12Lib::SetSamplerDesc(0, D3D12_FILTER_MIN_MAG_MIP_LINEAR);
	samplerDesc[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	samplerDesc[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	samplerDesc[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;

	// ルートシグネチャの設定
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	rootSignatureDesc.pParameters = rootparams;
	rootSignatureDesc.NumParameters = _countof(rootparams);

	rootSignatureDesc.pStaticSamplers = samplerDesc;
	rootSignatureDesc.NumStaticSamplers = _countof(samplerDesc);

	// バージョン自動判定のシリアライズ
	result = D3D12SerializeRootSignature(
		&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
	if (FAILED(result)) {
		Log(reinterpret_cast<char*>(errorBlob->GetBufferPointer()));
		assert(false);
	}

	// ルートシグネチャの生成
	result = DirectXCommon::GetInstance()->GetDevice()->CreateRootSignature(
		0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&sRootSignature_));
	assert(SUCCEEDED(result));

	gpipeline.pRootSignature = sRootSignature_.Get();

#pragma region Blend
// ブレンドなし
	D3D12_BLEND_DESC blenddesc{};
	blenddesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	blenddesc.RenderTarget[0].BlendEnable = false;
	gpipeline.BlendState = blenddesc;
	// グラフィックスパイプラインの生成
	result =
		DirectXCommon::GetInstance()->GetDevice()->CreateGraphicsPipelineState(
			&gpipeline, IID_PPV_ARGS(&sPipelineStates_[size_t(BlendMode::kNone)]));
	assert(SUCCEEDED(result));

	// αブレンド
	blenddesc = SetBlendDesc(D3D12_BLEND_SRC_ALPHA, D3D12_BLEND_OP_ADD, D3D12_BLEND_INV_SRC_ALPHA);

	// ブレンドステート
	gpipeline.BlendState = blenddesc;
	// グラフィックスパイプラインの生成
	result =
		DirectXCommon::GetInstance()->GetDevice()->CreateGraphicsPipelineState(
			&gpipeline, IID_PPV_ARGS(&sPipelineStates_[size_t(BlendMode::kNormal)]));
	assert(SUCCEEDED(result));

	// 加算合成
	blenddesc = SetBlendDesc(D3D12_BLEND_SRC_ALPHA, D3D12_BLEND_OP_ADD, D3D12_BLEND_ONE);
	// ブレンドステート
	gpipeline.BlendState = blenddesc;
	// グラフィックスパイプラインの生成
	result =
		DirectXCommon::GetInstance()->GetDevice()->CreateGraphicsPipelineState(
			&gpipeline, IID_PPV_ARGS(&sPipelineStates_[size_t(BlendMode::kAdd)]));
	assert(SUCCEEDED(result));

	// 減算合成
	blenddesc = SetBlendDesc(D3D12_BLEND_SRC_ALPHA, D3D12_BLEND_OP_REV_SUBTRACT, D3D12_BLEND_ONE);
	// ブレンドステート
	gpipeline.BlendState = blenddesc;
	// グラフィックスパイプラインの生成
	result =
		DirectXCommon::GetInstance()->GetDevice()->CreateGraphicsPipelineState(
			&gpipeline, IID_PPV_ARGS(&sPipelineStates_[size_t(BlendMode::kSubtract)]));
	assert(SUCCEEDED(result));

	// 乗算合成
	blenddesc = SetBlendDesc(D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD, D3D12_BLEND_SRC_COLOR);
	// ブレンドステート
	gpipeline.BlendState = blenddesc;
	// グラフィックスパイプラインの生成
	result =
		DirectXCommon::GetInstance()->GetDevice()->CreateGraphicsPipelineState(
			&gpipeline, IID_PPV_ARGS(&sPipelineStates_[size_t(BlendMode::kMultiply)]));
	assert(SUCCEEDED(result));

	// スクリーン合成
	blenddesc = SetBlendDesc(D3D12_BLEND_INV_DEST_COLOR, D3D12_BLEND_OP_ADD, D3D12_BLEND_ONE);
	// ブレンドステート
	gpipeline.BlendState = blenddesc;
	// グラフィックスパイプラインの生成
	result =
		DirectXCommon::GetInstance()->GetDevice()->CreateGraphicsPipelineState(
			&gpipeline, IID_PPV_ARGS(&sPipelineStates_[size_t(BlendMode::kScreen)]));
	assert(SUCCEEDED(result));
#pragma endregion

}

D3D12_RASTERIZER_DESC PipelineManager::SetRasterizerState(D3D12_FILL_MODE fillMode, D3D12_CULL_MODE cullMode)
{
	D3D12_RASTERIZER_DESC rasterizer{};
	rasterizer.FillMode = fillMode;
	rasterizer.CullMode = cullMode;
	rasterizer.FrontCounterClockwise = FALSE;
	rasterizer.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
	rasterizer.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
	rasterizer.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
	rasterizer.DepthClipEnable = TRUE;
	rasterizer.MultisampleEnable = FALSE;
	rasterizer.AntialiasedLineEnable = FALSE;
	rasterizer.ForcedSampleCount = 0;
	rasterizer.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;
	return rasterizer;
}

D3D12_INPUT_ELEMENT_DESC PipelineManager::SetInputLayout(const char* semanticName, DXGI_FORMAT format)
{
    D3D12_INPUT_ELEMENT_DESC inputLayout = {
        semanticName,0,format,0,D3D12_APPEND_ALIGNED_ELEMENT,
        D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
    };

    return inputLayout;
}

D3D12_BLEND_DESC PipelineManager::SetBlendDesc(D3D12_BLEND srcBlend, D3D12_BLEND_OP blendOp, D3D12_BLEND destBlend)
{
	D3D12_BLEND_DESC blendDesc{};
	// 初期設定
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	blendDesc.RenderTarget[0].BlendEnable = TRUE;

	// 基本設定
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
	// 種類別設定
	blendDesc.RenderTarget[0].SrcBlend = srcBlend;
	blendDesc.RenderTarget[0].BlendOp = blendOp;
	blendDesc.RenderTarget[0].DestBlend = destBlend;
	return blendDesc;
}

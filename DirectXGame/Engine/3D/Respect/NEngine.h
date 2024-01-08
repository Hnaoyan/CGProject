#pragma once
#include "DirectXCommon.h"
#include "WindowAPI.h"

class NEngine
{
public:

	static NEngine* GetInstance() {
		static NEngine instance;
		return &instance;
	}

	void Initialize(DirectXCommon* dxCommon, WindowAPI* winApi);

	enum BlendState {
		//通常のαブレンド
		kBlendModeNormal,
		//加算ブレンド
		kBlendModeAdd,
		//減産ブレンド
		kBlendModeSubtract,
		//乗算ブレンド
		kBlendModeMultiply,
		//スクリーンブレンド
		kBlendModeScreen,
	};

	void ParticlePreDraw(ID3D12GraphicsCommandList* commandList);

	void ParticlePostDraw();

private:
	void SetBlendState();
	void CreateRootSignature();
	void CreateInputlayout();
	void SettingRasterrizer();
	void InitPSO();

private:
	WindowAPI* winAPI_;
	DirectXCommon* dxCommon_ = nullptr;

	D3D12_DEPTH_STENCIL_DESC depthStencilDescParticle{};
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc_{};
	D3D12_BLEND_DESC blendDesc_[5]{};

	//Particle用のパイプライン
	Microsoft::WRL::ComPtr<ID3DBlob>signatureBlobParticle_;
	Microsoft::WRL::ComPtr<ID3DBlob>errorBlobParticle_;
	Microsoft::WRL::ComPtr<ID3D12RootSignature>rootSignatureParticle_;
	IDxcBlob* vertexShaderBlobParticle_;
	IDxcBlob* pixelShaderBlobParticle_;
	Microsoft::WRL::ComPtr<ID3D12PipelineState>graphicsPipelineStateParticle_;
	D3D12_RASTERIZER_DESC rasterizerDescParticle_{};
	D3D12_INPUT_ELEMENT_DESC inputElementDescsParticle_[3];

};


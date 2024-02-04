#pragma once
#include <array>
#include <memory>
#include <wrl.h>

#include <dxgidebug.h>
#include <dxcapi.h>
#pragma comment(lib, "dxcompiler.lib")

#include<d3d12.h>
#include<dxgi1_6.h>

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")


namespace Pipeline 
{
	enum class RootParameter : int {
		kWorldTransform,
		kViewProjection,
		kMaterial,
		kTexture,
		kLight,

		// サイズを取得する為の値
		kCountOfParameter,	
	};

	// パーティクルのレジスタ用
	enum class ParticleRegister : int {
		kWorldTransform,
		kViewProjection,
		kMaterial,
		kTexture,

		kCountOfParameter,
	};

	enum class BlendMode : int {
		kNone,
		kNormal,
		kAdd,
		kSubtract,
		kMultiply,
		kScreen,
		// サイズカウント
		kCountOfBlendMode,
	};

	enum class DrawType : int {
		kModel,
		kParticle,

		// サイズカウント
		kCountOfDrawer,
	};

}

class PipelineManager
{
public:
	static PipelineManager* GetInstance() {
		static PipelineManager instance;
		return &instance;
	}

	static void CreatePipeline();

public:
	// Model用
	static std::array<Microsoft::WRL::ComPtr<ID3D12PipelineState>,
		size_t(Pipeline::BlendMode::kCountOfBlendMode)> sPipelineStates_;
	static Microsoft::WRL::ComPtr<ID3D12RootSignature> sRootSignature_;

	// Particle用（インスタンシング
	static Microsoft::WRL::ComPtr<ID3D12PipelineState> sParticlePipelineStates_;
	static Microsoft::WRL::ComPtr<ID3D12RootSignature> sParticleRootSignature_;

public: // 作成関数
	static void CreateModelPipeline();
	static void CreateParticlePipeline();

public:
	static D3D12_RASTERIZER_DESC SetRasterizerState(D3D12_FILL_MODE fillMode, D3D12_CULL_MODE cullMode);
	static D3D12_INPUT_ELEMENT_DESC SetInputLayout(const char* semanticName,DXGI_FORMAT format);
	static D3D12_BLEND_DESC SetBlendDesc(D3D12_BLEND srcBlend, D3D12_BLEND_OP blendOp, D3D12_BLEND destBlend);
};


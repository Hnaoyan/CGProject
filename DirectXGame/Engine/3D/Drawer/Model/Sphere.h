#pragma once
#include <dxgidebug.h>
#pragma comment(lib,"dxguid.lib")

#include <dxcapi.h>
#pragma comment(lib, "dxcompiler.lib")

#include<d3d12.h>
#include<dxgi1_6.h>

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

#include "DirectXTex.h"
#include "d3dx12.h"

#include <wrl.h>

#include "WorldTransform.h"
#include "ViewProjection.h"


class Sphere
{
//public:
//	void CreatePipeline();
//
//	void CreateRootSignature();
//
//	static void PreDraw(ID3D12GraphicsCommandList* cmdList);
//
//	static void PostDraw();
//
//	void Draw(const ViewProjection& viewProjection);
//
//	void CreateMesh();
//
//	void Update();
//

private:
	static Microsoft::WRL::ComPtr<ID3D12RootSignature> sRootSignature_;
	static Microsoft::WRL::ComPtr<ID3D12PipelineState> sPipelineState_;

	uint32_t texture_ = 0u;

};


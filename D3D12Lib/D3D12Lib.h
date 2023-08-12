#pragma once
#include <wrl.h>
#include "../WinApp.h"

#include <dxgidebug.h>
#include <dxcapi.h>
#pragma comment(lib, "dxcompiler.lib")

#include<d3d12.h>
#include<dxgi1_6.h>

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

class D3D12Lib
{
public:
	static D3D12_HEAP_PROPERTIES SetHeapProperties(D3D12_HEAP_TYPE type);
	static D3D12_RESOURCE_DESC SetResourceDesc(UINT64 sizeInByte);
	static D3D12_STATIC_SAMPLER_DESC SetSamplerDesc(UINT shaderRegister, D3D12_FILTER filter);

	/// <summary>
	/// ディスクリプタレンジ初期化
	/// </summary>
	/// <param name="rangeType"></param>
	/// <param name="numDescriptors"></param>
	/// <param name="baseShaderRegister"></param>
	/// <returns></returns>
	static D3D12_DESCRIPTOR_RANGE Init(D3D12_DESCRIPTOR_RANGE_TYPE rangeType, UINT numDescriptors, UINT baseShaderRegister);

	static D3D12_ROOT_PARAMETER InitAsConstantBufferView(UINT shaderRegister, UINT registerSpace, D3D12_SHADER_VISIBILITY visibility);

	static D3D12_ROOT_PARAMETER InitAsDescriptorTable(UINT numDescriptorRanges, const D3D12_DESCRIPTOR_RANGE* pDescritorRange, D3D12_SHADER_VISIBILITY visibility);

	static IDxcBlob* CompileShader(
		// CompilerするShaderファイルへのパス
		const std::wstring& filePath,
		// Compilerに使用するProfile
		const wchar_t* profile,
		// 初期化で生成したものを3つ
		IDxcUtils* dxcUtils,
		IDxcCompiler3* dxcCompiler,
		IDxcIncludeHandler* includeHandler);

	// バイトコード設定
	static D3D12_SHADER_BYTECODE ShaderByteCode(IDxcBlob* blob);
	
	static D3D12_RESOURCE_DESC TexResourceDesc(DXGI_FORMAT format, UINT64 width, UINT height);

	static D3D12_RESOURCE_DESC TexResourceDesc(DXGI_FORMAT format, UINT64 width, UINT height, UINT16 arraySize, UINT16 mipLevels);
	// ヒーププロパティの設定
	static D3D12_HEAP_PROPERTIES SetTexHeapProp(D3D12_CPU_PAGE_PROPERTY prop, D3D12_MEMORY_POOL memoryPool);
	// CPUデスクリプタハンドルを取得
	static D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(const D3D12_CPU_DESCRIPTOR_HANDLE& other, uint32_t handle, UINT descriptorHandleIncrementSize);
	// GPUデスクリプタハンドルを取得
	static D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(const D3D12_GPU_DESCRIPTOR_HANDLE& other, uint32_t handle, UINT descriptorHandleIncrementSize);

	static D3D12_RASTERIZER_DESC SetRasterizer();

};



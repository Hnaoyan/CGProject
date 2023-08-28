#include "D3D12Lib.h"
#include "StringManager.h"
#include <cassert>

using namespace Microsoft::WRL;

ComPtr<IDxcUtils> D3D12Lib::dxcUtils_;
ComPtr<IDxcCompiler3> D3D12Lib::dxcCompiler_;
ComPtr<IDxcIncludeHandler> D3D12Lib::includeHandler_;

D3D12Lib* D3D12Lib::GetInstance()
{
	static D3D12Lib instance;
	return &instance;
}

D3D12_HEAP_PROPERTIES D3D12Lib::SetHeapProperties(D3D12_HEAP_TYPE type)
{
	D3D12_HEAP_PROPERTIES heapProp{};
	heapProp.Type = type;
	heapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	heapProp.CreationNodeMask = 1;
	heapProp.VisibleNodeMask = 1;
	return heapProp;
}

D3D12_RESOURCE_DESC D3D12Lib::SetResourceDesc(UINT64 sizeInByte)
{
	// 頂点リソースの設定
	D3D12_RESOURCE_DESC resourceDesc{};
	// バッファリソース。テクスチャの場合はまた別の設定をする
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resourceDesc.Alignment = 0;
	resourceDesc.Width = sizeInByte;	// リソースのサイズ
	// バッファの場合はこれらは1にする決まり
	resourceDesc.Height = 1;
	resourceDesc.DepthOrArraySize = 1;
	resourceDesc.MipLevels = 1;
	resourceDesc.Format = DXGI_FORMAT_UNKNOWN;
	resourceDesc.SampleDesc.Count = 1;
	resourceDesc.SampleDesc.Quality = 0;
	// バッファの場合はこれにする決まり
	resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	resourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
	return resourceDesc;
}

D3D12_RESOURCE_DESC D3D12Lib::TexResoruceDesc(DXGI_FORMAT format,  uint32_t width, uint32_t height, UINT16 arraySize, UINT16 mipLevels) {
	D3D12_RESOURCE_DESC resourceDesc{};

	resourceDesc.Format = format;	// DepthStencilとして両可能なフォーマット
	resourceDesc.Width = width;		// テクスチャの幅
	resourceDesc.Height = height;	// テクスチャの高さ
	resourceDesc.MipLevels = mipLevels;		// mipMapの数
	resourceDesc.DepthOrArraySize = arraySize;	// 奥行き or 配列Textureの配列数
	resourceDesc.SampleDesc.Count = 1;	// サンプリングカウント。1固定
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;	// 2次元
	resourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;	// DepthStencilとして使う通知
	resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;

	return resourceDesc;
}

//D3D12_RESOURCE_DESC D3D12Lib::TexResourceDesc(DXGI_FORMAT format, UINT64 width, UINT height)
//{
//	format;
//	width;
//	height;
//	return D3D12_RESOURCE_DESC();
//}
//
//D3D12_RESOURCE_DESC D3D12Lib::TexResourceDesc(DXGI_FORMAT format, UINT64 width, UINT height, UINT16 arraySize, UINT16 mipLevels)
//{
//	D3D12_RESOURCE_DESC resDesc;
//	resDesc = { D3D12_RESOURCE_DIMENSION_TEXTURE2D, 0, width, height, arraySize,
//			mipLevels, format, 1, 0, D3D12_TEXTURE_LAYOUT_UNKNOWN, D3D12_RESOURCE_FLAG_NONE };
//	return resDesc;
//}


D3D12_STATIC_SAMPLER_DESC D3D12Lib::SetSamplerDesc(UINT shaderRegister, D3D12_FILTER filter)
{
	D3D12_STATIC_SAMPLER_DESC staticSampler{};
	staticSampler.ShaderRegister = shaderRegister;
	staticSampler.Filter = filter;
	staticSampler.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSampler.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSampler.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSampler.MipLODBias = 0;
	staticSampler.MaxAnisotropy = 16;
	staticSampler.ComparisonFunc = D3D12_COMPARISON_FUNC_EQUAL;
	staticSampler.BorderColor = D3D12_STATIC_BORDER_COLOR_OPAQUE_WHITE;
	staticSampler.MinLOD = 0.f;
	staticSampler.MaxLOD = D3D12_FLOAT32_MAX;
	staticSampler.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	staticSampler.RegisterSpace = 0;
	return staticSampler;
}

D3D12_DESCRIPTOR_RANGE D3D12Lib::Init(D3D12_DESCRIPTOR_RANGE_TYPE rangeType, UINT numDescriptors, UINT baseShaderRegister)
{
	D3D12_DESCRIPTOR_RANGE ranges{};
	ranges.RangeType = rangeType;
	ranges.NumDescriptors = numDescriptors;
	ranges.BaseShaderRegister = baseShaderRegister;
	ranges.RegisterSpace = 0;
	ranges.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
	return ranges;
}

D3D12_ROOT_PARAMETER D3D12Lib::InitAsConstantBufferView(UINT shaderRegister, UINT registerSpace, D3D12_SHADER_VISIBILITY visibility)
{
	D3D12_ROOT_PARAMETER rootParam{};
	rootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParam.ShaderVisibility = visibility;
	rootParam.Descriptor.RegisterSpace = registerSpace;
	rootParam.Descriptor.ShaderRegister = shaderRegister;
	return rootParam;
}

D3D12_ROOT_PARAMETER D3D12Lib::InitAsDescriptorTable(UINT numDescriptorRanges, const D3D12_DESCRIPTOR_RANGE* pDescritorRange, D3D12_SHADER_VISIBILITY visibility)
{
	D3D12_ROOT_PARAMETER rootParam{};
	rootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParam.ShaderVisibility = visibility;
	rootParam.DescriptorTable.NumDescriptorRanges = numDescriptorRanges;
	rootParam.DescriptorTable.pDescriptorRanges = pDescritorRange;
	return rootParam;
}

IDxcBlob* D3D12Lib::CompileShader(const std::wstring& filePath, const wchar_t* profile)
{
	CreateDXC();
	// ここの中身をこの後書く
		// 1.hlslファイルを読む
		// これからシェーダーをコンパイルする旨をログに出す
	Log(ConvertString(std::format(L"Begin CompileShader, path:{}, profile:{}\n", filePath, profile)));
	// hlslファイルを読む
	IDxcBlobEncoding* shaderSource = nullptr;
	HRESULT hr = dxcUtils_->LoadFile(filePath.c_str(), nullptr, &shaderSource);
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
	hr = dxcCompiler_->Compile(
		&shaderSourceBuffer,	// 読み込んだファイル
		arguments,				// コンパイルオプション
		_countof(arguments),	// コンパイルオプションの数
		includeHandler_.Get(),			// includeが含まれた諸々
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

D3D12_SHADER_BYTECODE D3D12Lib::ShaderByteCode(IDxcBlob* blob)
{
	D3D12_SHADER_BYTECODE shaderByte = { blob->GetBufferPointer(),blob->GetBufferSize() };
	return shaderByte;
}


D3D12_HEAP_PROPERTIES D3D12Lib::SetTexHeapProp(D3D12_CPU_PAGE_PROPERTY prop, D3D12_MEMORY_POOL memoryPool)
{
	D3D12_HEAP_PROPERTIES heapProps;
	heapProps.Type = D3D12_HEAP_TYPE_CUSTOM;
	heapProps.CPUPageProperty = prop;
	heapProps.MemoryPoolPreference = memoryPool;
	heapProps.CreationNodeMask = 1;
	heapProps.VisibleNodeMask = 1;
	return heapProps;
}

D3D12_CPU_DESCRIPTOR_HANDLE D3D12Lib::GetCPUDescriptorHandle(const D3D12_CPU_DESCRIPTOR_HANDLE& other, uint32_t handle, UINT descriptorHandleIncrementSize)
{
	D3D12_CPU_DESCRIPTOR_HANDLE handle_;
	handle_.ptr = SIZE_T(INT64(other.ptr) + INT64(handle) * INT(descriptorHandleIncrementSize));
	return handle_;
}

D3D12_GPU_DESCRIPTOR_HANDLE D3D12Lib::GetGPUDescriptorHandle(const D3D12_GPU_DESCRIPTOR_HANDLE& other, uint32_t handle, UINT descriptorHandleIncrementSize)
{
	D3D12_GPU_DESCRIPTOR_HANDLE handle_;
	handle_.ptr = SIZE_T(INT64(other.ptr) + INT64(handle) * INT(descriptorHandleIncrementSize));
	return handle_;
}

D3D12_RASTERIZER_DESC D3D12Lib::SetRasterizer()
{
	D3D12_RASTERIZER_DESC rasterizer{};
	rasterizer.FillMode = D3D12_FILL_MODE_SOLID;
	rasterizer.CullMode = D3D12_CULL_MODE_BACK;
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

void D3D12Lib::CreateDXC()
{
	HRESULT result = S_FALSE;
	result = DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&dxcUtils_));

	assert(SUCCEEDED(result));
	result = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&dxcCompiler_));
	assert(SUCCEEDED(result));

	result = dxcUtils_->CreateDefaultIncludeHandler(includeHandler_.GetAddressOf());
	assert(SUCCEEDED(result));
}


#pragma once
#include <wrl.h>
#include "WinApp.h"
#include "Matrix.h"

#include <dxgidebug.h>
#include <dxcapi.h>
#pragma comment(lib, "dxcompiler.lib")

#include<d3d12.h>
#include<dxgi1_6.h>

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")


class Sprite
{
public:	// サブクラス
	struct Vector4 {
		float x, y, z, w;
	};
	
	struct Vector3 {
		float x, y, z;
	};

	struct TransformationMatrix {
		Matrix4x4 WVP;
	};

	struct Transform {
		Vector3 scale;
		Vector3 rotate;
		Vector3 translate;
	};

public:	// 静的メンバ関数

	static void StaticInitialize(ID3D12Device* device);

	static void PreDraw(ID3D12GraphicsCommandList* commandList);

	static void PostDraw();

	static void InitializeDXC();

	static Sprite* Create();

private:	// 静的メンバ変数

	static ID3D12Device* sDevice_;

	static ID3D12GraphicsCommandList* sCommandList_;

	static Microsoft::WRL::ComPtr<ID3D12RootSignature> sRootSignature_;

	static Microsoft::WRL::ComPtr<IDxcUtils> dxcUtils_;

	static Microsoft::WRL::ComPtr<IDxcCompiler3> dxcCompiler_;

	static Microsoft::WRL::ComPtr<IDxcIncludeHandler> includeHandler_;

	static Microsoft::WRL::ComPtr<ID3D12PipelineState> gPipelineState_;


public:

	Sprite() {};

	bool Initialize();

	void Draw();

	void SetWvpMatrix(Matrix4x4 wvp) { wvpMatrix_ = wvp; }

private:
		
	static IDxcBlob* CompileShader(
		// CompilerするShaderファイルへのパス
		const std::wstring& filePath,
		// Compilerに使用するProfile
		const wchar_t* profile,
		// 初期化で生成したものを3つ
		IDxcUtils* dxcUtils,
		IDxcCompiler3* dxcCompiler,
		IDxcIncludeHandler* includeHandler);

	static D3D12_SHADER_BYTECODE ShaderByteCode(IDxcBlob* blob);

	static ID3D12Resource* CreateBufferResoruce(size_t sizeInBytes);

private:	// メンバ関数

	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff_;

	Microsoft::WRL::ComPtr<ID3D12Resource> constBuff_;

	Vector4* vertData_ = nullptr;

	Vector4* constData_ = nullptr;

	Matrix4x4 wvpMatrix_;

	Microsoft::WRL::ComPtr<ID3D12Resource> wvpResoure_;

	TransformationMatrix* wvpData = nullptr;

	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vertBufferView_{};
};


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

	struct Vector2 {
		float x, y;
	};

	struct TransformationMatrix {
		Matrix4x4 WVP;
	};

	struct Transform {
		Vector3 scale;
		Vector3 rotate;
		Vector3 translate;
	};

	struct VertexData {
		Vector4 position;
		Vector2 texcoord;
	};

	struct ConstBufferData {
		Vector4 color;
	};

public:	// 静的メンバ関数

	/// <summary>
	/// 静的初期化
	/// </summary>
	/// <param name="device"></param>
	static void StaticInitialize(ID3D12Device* device);

	/// <summary>
	/// 描画前処理
	/// </summary>
	/// <param name="blob"></param>
	/// <returns></returns>
	static void PreDraw(ID3D12GraphicsCommandList* commandList);

	/// <summary>
	/// 描画後処理
	/// </summary>
	static void PostDraw();

	/// <summary>
	/// DXCompilerの初期化
	/// </summary>
	static void InitializeDXC();

	/// <summary>
	/// リソース作成
	/// </summary>
	/// <returns></returns>
	static Sprite* Create();

private:	// 静的メンバ変数

	static ID3D12Device* sDevice_;

	static ID3D12GraphicsCommandList* sCommandList_;

	static Microsoft::WRL::ComPtr<ID3D12RootSignature> sRootSignature_;

	static Microsoft::WRL::ComPtr<IDxcUtils> dxcUtils_;

	static Microsoft::WRL::ComPtr<IDxcCompiler3> dxcCompiler_;

	static Microsoft::WRL::ComPtr<IDxcIncludeHandler> includeHandler_;

	static Microsoft::WRL::ComPtr<ID3D12PipelineState> gPipelineState_;
	// デスクリプタサイズ
	static UINT sDescriptorHandleIncrementSize_;
	// 頂点数
	static const int kVertNum = 6;

public:

	/// <summary>
	/// new
	/// </summary>
	Sprite() {};

	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns></returns>
	bool Initialize();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// WorldViewportMatrixのSetter
	/// </summary>
	void SetWvpMatrix(Matrix4x4 wvp) { wvpMatrix_ = wvp; }

	void SetWvpSpriteMatrix(Matrix4x4 wvp) { wvpSpriteMat_ = wvp; }

	void SetIsTriangle(bool isTriangle) { IsTriangle_ = isTriangle; }

	void SetIsSprite(bool isSprite) { IsSprite_ = isSprite; }

	void SetPattern(int patt) { pattern = patt; }

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

	static Microsoft::WRL::ComPtr<ID3D12Resource> CreateBufferResoruce(size_t sizeInBytes);

	D3D12_RESOURCE_DESC SetResourceDesc(size_t size);

private:	// メンバ関数

	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff_;

	Microsoft::WRL::ComPtr<ID3D12Resource> constBuff_;

	VertexData* vertData_ = nullptr;

	Vector4* constData_ = nullptr;

	Matrix4x4 wvpMatrix_;

	Microsoft::WRL::ComPtr<ID3D12Resource> wvpResoure_;

	TransformationMatrix* wvpData = nullptr;



	Microsoft::WRL::ComPtr<ID3D12Resource> vertSpriteBuff_;

	VertexData* vertSpriteData_ = nullptr;

	Matrix4x4 wvpSpriteMat_;

	Microsoft::WRL::ComPtr<ID3D12Resource> wvpSpriteResource_;

	TransformationMatrix* wvpSpriteData_ = nullptr;

	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vertBufferView_{};

	D3D12_VERTEX_BUFFER_VIEW vertSpriteBufferView_{};

	UINT textureHandle_ = 0;

	bool IsTriangle_ = 1;

	bool IsSprite_ = 0;

	int pattern = PatternUp;

};


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
	struct TransformationMatrix {
		Matrix4x4 WVP;
		Matrix4x4 World;
	};

	struct DirectionalLight {
		Vector4 color;
		Vector3 direction;
		float intensity;
	};

	//struct Transform {
	//	Vector3 scale;
	//	Vector3 rotate;
	//	Vector3 translate;
	//};

	struct VertexData {
		Vector4 position;
		Vector2 texcoord;
		Vector3 normal;
	};

	struct ConstBufferData {
		Vector4 color;
		int32_t enableLighting;
	};

	enum LightingPattern {
		None,
		Lambertian,
		Half,
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

public:
	/// <summary>
	/// WorldViewportMatrixのSetter
	/// </summary>
	void SetWvpMatrix(Matrix4x4 wvp) { wvpMatrix_ = wvp; }

	void SetWvpSpriteMatrix(Matrix4x4 wvp) { wvpSpriteMat_ = wvp; }

	void SetWvpSphereMatrix(Matrix4x4 wvp) { wvpSphereMatrix_ = wvp; }

	/// <summary>
	/// 表示フラグのセッター
	/// </summary>
	/// <param name="isTriangle"></param>
	void SetIsTriangle(bool isTriangle) { IsTriangle_ = isTriangle; }

	void SetIsSprite(bool isSprite) { IsSprite_ = isSprite; }

	void SetIsSphere(bool isSphere) { IsSphere_ = isSphere; }

	/// <summary>
	/// 色のセッター
	/// </summary>
	/// <param name="color"></param>
	void SetColor(Vector4 color) { color_ = color; }

	/// <summary>
	/// ワールド座標
	/// </summary>
	/// <param name="world"></param>
	void SetWorldMat(Matrix4x4 world) { worldMat_ = world; }

	void SetWorldSphereMat(Matrix4x4 world) { worldSphereMat_ = world; }

	/// <summary>
	/// ライティング系
	/// </summary>
	/// <param name="light"></param>
	void SetLighting(DirectionalLight light) { *directionalLightData_ = light; }

	DirectionalLight GetLighting() { return *directionalLightData_; }

	int GetLightPattern() { return lightPattern_; }

	void SetLightPattern(int lightpattern) { lightPattern_ = lightpattern; }

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

	static Microsoft::WRL::ComPtr<ID3D12Resource> CreateBufferResource(size_t sizeInBytes);

	D3D12_RESOURCE_DESC SetResourceDesc(size_t size);

private:	// メンバ関数

	/// <summary>
	/// 定数バッファ
	/// </summary>
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuff_;

	ConstBufferData* constData_ = nullptr;

	Microsoft::WRL::ComPtr<ID3D12Resource> constSpriteBuff_;

	ConstBufferData* constSpriteData_ = nullptr;

	Microsoft::WRL::ComPtr<ID3D12Resource> constSphereBuff_;

	ConstBufferData* constSphereData_ = nullptr;

	Microsoft::WRL::ComPtr<ID3D12Resource> directionalLightBuff_;

	DirectionalLight* directionalLightData_ = nullptr;

	/// <summary>
	/// 三角形のバッファ
	/// </summary>
	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff_;

	VertexData* vertData_ = nullptr;

	Matrix4x4 wvpMatrix_;

	Matrix4x4 worldMat_;

	Microsoft::WRL::ComPtr<ID3D12Resource> wvpResoure_;

	TransformationMatrix* wvpData = nullptr;

	/// <summary>
	/// スプライトのバッファ
	/// </summary>
	Microsoft::WRL::ComPtr<ID3D12Resource> vertSpriteBuff_;

	VertexData* vertSpriteData_ = nullptr;

	Matrix4x4 wvpSpriteMat_;

	Microsoft::WRL::ComPtr<ID3D12Resource> wvpSpriteResource_;

	TransformationMatrix* wvpSpriteData_ = nullptr;

	Microsoft::WRL::ComPtr<ID3D12Resource> indexSpriteBuff_;

	D3D12_INDEX_BUFFER_VIEW indexSpriteBufferView_{};

	uint32_t* indexSpriteData = nullptr;
	
	/// <summary>
	/// 球体用
	/// </summary>
	Microsoft::WRL::ComPtr<ID3D12Resource> vertSphereBuff_;

	VertexData* vertSphereData_ = nullptr;

	Matrix4x4 wvpSphereMatrix_;

	Matrix4x4 worldSphereMat_;

	Microsoft::WRL::ComPtr<ID3D12Resource> wvpSphereResource_;

	TransformationMatrix* wvpSphereData_ = nullptr;
	// 分割数
	uint32_t kSubdivision = 512;
	// 頂点数
	uint32_t kVertexIndex = kSubdivision * kSubdivision * 6;

	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vertBufferView_{};

	D3D12_VERTEX_BUFFER_VIEW vertSpriteBufferView_{};

	D3D12_VERTEX_BUFFER_VIEW vertSphereBufferView_{};

	UINT textureHandle_ = 0;

	bool IsTriangle_ = 1;

	bool IsSprite_ = 0;

	bool IsSphere_ = 0;

	Vector4 color_ = {1.0f,1.0f,1.0f,1.0f};
	
	int lightPattern_ = LightingPattern::None;

};


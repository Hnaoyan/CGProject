#pragma once
#include <wrl.h>
#include "WinApp.h"
#include "Matrix.h"
#include "StructManager.h"

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

	struct VertexData {
		Vector3 position;
		Vector2 texcoord;
	};

	struct ConstBufferData {
		Vector4 color;
		Matrix4x4 mat;
	};

public:	// 静的メンバ関数

	/// <summary>
	/// 静的初期化
	/// </summary>
	/// <param name="device"></param>
	static void StaticInitialize(ID3D12Device* device, int window_width, int window_height);

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
	static Sprite* Create(uint32_t textureHandle, Vector2 position, Vector4 color, Vector2 anchorpoint, bool isFlipX, bool isFlipY);

private:	// 静的メンバ変数

	static ID3D12Device* sDevice_;

	static ID3D12GraphicsCommandList* sCommandList_;

	static Microsoft::WRL::ComPtr<ID3D12RootSignature> sRootSignature_;

	static Microsoft::WRL::ComPtr<ID3D12PipelineState> gPipelineState_;
	// デスクリプタサイズ
	static UINT sDescriptorHandleIncrementSize_;
	// 頂点数
	static const int kVertNum = 4;

	static Matrix4x4 sMatProjection_;

public:

	/// <summary>
	/// new
	/// </summary>
	Sprite() {};

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="textureHandle"></param>
	/// <param name="position"></param>
	/// <param name="color"></param>
	/// <param name="anchorpoint"></param>
	Sprite(uint32_t textureHandle, Vector2 position, Vector2 size, Vector4 color, Vector2 anchorpoint, bool isFlipX, bool isFlipY);

	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns></returns>
	bool Initialize();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	void SetWorldMatrix(Matrix4x4 wvp) { matWorld_ = wvp; }

private:
		
	static D3D12_SHADER_BYTECODE ShaderByteCode(IDxcBlob* blob);

	static Microsoft::WRL::ComPtr<ID3D12Resource> CreateBufferResource(size_t sizeInBytes);

	D3D12_RESOURCE_DESC SetResourceDesc(size_t size);

private:	// メンバ関数

	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff_;

	Microsoft::WRL::ComPtr<ID3D12Resource> constBuff_;
	// 頂点バッファ
	VertexData* vertData_ = nullptr;
	// 定数バッファ
	ConstBufferData* constData_ = nullptr;

	Microsoft::WRL::ComPtr<ID3D12Resource> wvpResoure_;

	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vertBufferView_{};

	// テクスチャ
	UINT textureHandle_ = 0;

	// テクスチャの始点
	Vector2 texBase_{ 0,0 };
	// 座標
	Vector2 position_{};

	// スプライトのサイズ
	Vector2 size_ = { 100.0f,100.0f };

	// アンカーポイント
	Vector2 anchorpoint_{};

	// ワールド行列
	Matrix4x4 matWorld_{};

	// 色
	Vector4 color_ = { 1,1,1,1 };

	float rotation_ = 0;

	// テクスチャのサイズ
	Vector2 texSize_ = { 100.0f,100.0f };
	// 左右反転
	bool isFlipX_ = false;
	// 上下反転
	bool isFlipY_ = false;

	D3D12_RESOURCE_DESC resourceDesc_;

private:

	void TransferVertices();


};


#pragma once
#include <wrl.h>
#include <array>
#include "WindowAPI.h"
#include "StructManager.h"
#include "Graphics/PipelineManager.h"

#include <dxgidebug.h>
#include <dxcapi.h>
#pragma comment(lib, "dxcompiler.lib")

#include<d3d12.h>
#include<dxgi1_6.h>

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

struct SpriteInfo {
	Vector2 position;
	Vector4 color;
	Vector2 anchor;
};

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
		Matrix4x4 uv;
	};

	struct Transform {
		Vector3 scale;
		Vector3 rotate;
		Vector3 translate;
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
	/// リソース作成
	/// </summary>
	/// <returns></returns>
	static Sprite* Create(uint32_t textureHandle, Vector2 position, Vector4 color, Vector2 anchorpoint, bool isFlipX, bool isFlipY);

private:	// 静的メンバ変数
	// 頂点数
	static const int kVertNum = 4;

	static ID3D12Device* sDevice_;

	static ID3D12GraphicsCommandList* sCommandList_;

	static Microsoft::WRL::ComPtr<ID3D12RootSignature> sRootSignature_;

	// パイプラインステートオブジェ
	static std::array<Microsoft::WRL::ComPtr<ID3D12PipelineState>,
		size_t(Pipeline::BlendMode::kCountOfBlendMode)> sPipelineStates_;

	//static Microsoft::WRL::ComPtr<ID3D12PipelineState> gPipelineState_;
	// デスクリプタサイズ
	//static UINT sDescriptorHandleIncrementSize_;

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
	Sprite(uint32_t textureHandle, Vector2 position,
		Vector2 size, Vector4 color, Vector2 anchorpoint, bool isFlipX, bool isFlipY);

	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns></returns>
	bool Initialize();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// ワールド行列の設定
	/// </summary>
	/// <param name="wvp"></param>
	void SetWorldMatrix(Matrix4x4 wvp) { matWorld_ = wvp; }

public:

	/// <summary>
	/// テクスチャの設定
	/// </summary>
	void SetTextureHandle(uint32_t textureHandle);

	uint32_t GetTextureHandle() { return textureHandle_; }

	/// <summary>
	/// 座標の設定
	/// </summary>
	/// <param name="position"></param>
	void SetPosition(const Vector2& position);

	const Vector2& GetPosition() { return position_; }

	/// <summary>
	/// サイズの設定
	/// </summary>
	/// <param name="size"></param>
	void SetSize(const Vector2& size);

	const Vector2& GetSize() { return size_; }

	/// <summary>
	/// 角度の設定
	/// </summary>
	/// <param name="rotation"></param>
	void SetRotation(float rotation);

	float GetRotation() { return rotation_; }

	/// <summary>
	/// アンカーポイントの設定
	/// </summary>
	/// <param name="anchorPoint"></param>
	void SetAnchorPoint(const Vector2& anchorPoint);

	const Vector2 GetAnchorPoint() { return anchorpoint_; }

	/// <summary>
	/// 色の設定
	/// </summary>
	/// <param name="color"></param>
	void SetColor(const Vector4& color) {
		color_ = color;
		Update();
	}

	const Vector4 GetColor() { return color_; }

	/// <summary>
	/// ブレンドの設定
	/// </summary>
	/// <param name="mode"></param>
	void SetBlendMode(Pipeline::BlendMode mode) { blendMode_ = mode; }

	/// <summary>
	/// 表示か非表示かの設定
	/// </summary>
	/// <param name="isInv"></param>
	void SetInvisible(bool isInv) { isInvisible_ = isInv; }

	/// <summary>
	/// 描画サイズ設定
	/// </summary>
	/// <param name="texBase"></param>
	/// <param name="texSize"></param>
	void SetSpriteRect(const Vector2& texBase, const Vector2& texSize);

private:

	void TransferVertices();

	void UVUpdate();

private:	// メンバ関数

	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff_;

	Microsoft::WRL::ComPtr<ID3D12Resource> constBuff_;
	// 頂点バッファ
	VertexData* vertData_ = nullptr;
	// 定数バッファ
	ConstBufferData* constData_ = nullptr;

	Microsoft::WRL::ComPtr<ID3D12Resource> wvpResoure_;

	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView_{};

	// テクスチャ
	UINT textureHandle_ = 0;

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

	// UV
	Transform UVTransform_ = {
		{1.0f,1.0f,1.0f},
		{0,0,0},
		{0,0,0}
	};

	float rotation_ = 0;

	// テクスチャの始点
	Vector2 texBase_{ 0,0 };

	// テクスチャのサイズ
	Vector2 texSize_ = { 100.0f,100.0f };
	// 左右反転
	bool isFlipX_ = false;
	// 上下反転
	bool isFlipY_ = false;

	bool isInvisible_ = false;

	D3D12_RESOURCE_DESC resourceDesc_{};

	Pipeline::BlendMode blendMode_ = Pipeline::BlendMode::kNormal;
};


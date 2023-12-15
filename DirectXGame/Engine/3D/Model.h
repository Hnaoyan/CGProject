#pragma once
#include "StructManager.h"
#include "Material.h"
#include "Mesh.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include <vector>
#include <array>
#include <string>
#include <memory>
#include <wrl.h>
#include <unordered_map>
#include "LightGroup.h"

#include <dxgidebug.h>
#include <dxcapi.h>
#pragma comment(lib, "dxcompiler.lib")

#include<d3d12.h>
#include<dxgi1_6.h>

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

class Model
{
public:
	enum class RootParameter : int{
		kWorldTransform,
		kViewProjection,
		kMaterial,
		kTexture,
		kLight,
		kCountOfParameter,	// サイズを取得する為の値
	};

	enum class BlendMode : int {
		kNone,
		kNormal,
		kAdd,
		kSubtract,
		kMultiply,
		kScreen,
		kCountOfBlendMode,
	};

private:
	static const std::string kBaseDirectory;
	static const std::string kDefaultName;

private: // パイプラインの設定
	// デスクリプタサイズ
	static UINT sDescriptorHandleIncrementSize_;
	// コマンドリスト
	static ID3D12GraphicsCommandList* sCommandList_;
	// ルートシグネチャ
	static Microsoft::WRL::ComPtr<ID3D12RootSignature> sRootSignature_;
	// パイプラインステートオブジェ
	static std::array<Microsoft::WRL::ComPtr<ID3D12PipelineState>,
		size_t(BlendMode::kCountOfBlendMode)> sPipelineStates_;
	// ライト
	static std::unique_ptr<LightGroup> lightGroup_;

private: // モデル管理の内訳
	// 名前
	std::string name_;
	// メッシュ
	std::vector<Mesh*> meshes_;
	// マテリアルコンテナ
	std::unordered_map<std::string, Material*> materials_;
	// デフォルトマテリアル
	Material* defaultMaterial_ = nullptr;
	// ブレンド
	BlendMode blendMode_ = BlendMode::kNormal;

	float alphaValue_ = 1.0f;

public:
	/// <summary>
	/// 静的初期化
	/// </summary>
	static void StaticInitialize();

	/// <summary>
	/// グラフィックスの初期化
	/// </summary>
	static void InitializeGraphicsPipeline();

	/// <summary>
	/// モデル生成
	/// </summary>
	/// <returns></returns>
	static Model* Create();

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	static Model* CreatePlane();

	/// <summary>
	/// OBJファイルからメッシュ生成
	/// </summary>
	/// <param name="modelName"></param>
	/// <param name="smoothing"></param>
	/// <returns></returns>
	static Model* CreateFromObj(const std::string& modelName, bool smoothing = false);

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

public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Model();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="modelName"></param>
	/// <param name="smoothing"></param>
	void Initialize(const std::string& modelName, bool smoothing = false);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// アルファ値の設定
	/// </summary>
	/// <param name="alpha"></param>
	void SetAlphaValue(float alpha);

	float GetAlphaValue();

	void SetBlendMode(BlendMode blendMode) { blendMode_ = blendMode; }

	void SetMaterial(Material* material);

	/// <summary>
	/// モデル描画
	/// </summary>
	/// <param name="worldTransform"></param>
	/// <param name="viewProjection"></param>
	void Draw(const WorldTransform& worldTransform, const ViewProjection& viewProjection);

	/// <summary>
	/// モデル描画（テクスチャが別
	/// </summary>
	/// <param name="worldTransform"></param>
	/// <param name="viewProjection"></param>
	/// <param name="textureHandle"></param>
	void Draw(const WorldTransform& worldTransform, const ViewProjection& viewProjection, UINT textureHandle);

private:	// メンバ関数

	/// <summary>
	/// モデル読み込み
	/// </summary>
	/// <param name="modelName"></param>
	/// <param name="smoothing"></param>
	void LoadModel(const std::string& modelName, bool smoothing);

	/// <summary>
	/// マテリアル読み込み
	/// </summary>
	/// <param name="directoryPath"></param>
	/// <param name="fileName"></param>
	void LoadMaterial(const std::string& directoryPath, const std::string& fileName);

	/// <summary>
	/// マテリアルの追加
	/// </summary>
	/// <param name="material"></param>
	void AddMaterial(Material* material);

	/// <summary>
	/// テクスチャ読み込み
	/// </summary>
	void LoadTextures();

};


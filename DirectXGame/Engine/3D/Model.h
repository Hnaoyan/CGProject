#pragma once
#include "StructManager.h"
#include "Material.h"
#include "Mesh.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include <vector>
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
	enum class RootParameter {
		kWorldTransform,
		kViewProjection,
		kMaterial,
		kTexture,
		kLight,
	};


private:
	static const std::string kBaseDirectory;
	static const std::string kDefaultName;

private:	// 静的メンバ変数
	// デスクリプタサイズ
	static UINT sDescriptorHandleIncrementSize_;
	// コマンドリスト
	static ID3D12GraphicsCommandList* sCommandList_;
	// ルートシグネチャ
	static Microsoft::WRL::ComPtr<ID3D12RootSignature> sRootSignature_;
	// パイプラインステートオブジェ
	static Microsoft::WRL::ComPtr<ID3D12PipelineState> sPipelineState_;
	// ライト
	static std::unique_ptr<LightGroup> lightGroup_;

private:
	// 名前
	std::string name_;
	// メッシュ
	std::vector<Mesh*> meshes_;
	// マテリアルコンテナ
	std::unordered_map<std::string, Material*> materials_;
	// デフォルトマテリアル
	Material* defaultMaterial_ = nullptr;

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

	void Initialize(const std::string& modelName, bool smoothing = false);

	void Draw(const WorldTransform& worldTransform, const ViewProjection& viewProjection);

	void Draw(const WorldTransform& worldTransform, const ViewProjection& viewProjection, UINT textureHandle);

private:	// メンバ関数

	/// <summary>
	/// モデル読み込み
	/// </summary>
	/// <param name="modelName"></param>
	/// <param name="smoothing"></param>
	void LoadModel(const std::string& modelName,bool smoothing);

	/// <summary>
	/// マテリアル読み込み
	/// </summary>
	/// <param name="directoryPath"></param>
	/// <param name="fileName"></param>
	void LoadMaterial(const std::string& directoryPath,const std::string& fileName);

	void AddMaterial(Material* material);

	/// <summary>
	/// テクスチャ読み込み
	/// </summary>
	void LoadTextures();

};


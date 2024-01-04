#pragma once
#include "StructManager.h"
#include "Model.h"
#include <memory>

class Particle
{
private:
	static const std::string kBaseDirectory;
	static const std::string kDefaultName;

public:
	static void StaticInitialize();

	static Particle* Create();

public:
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

	void PreDraw(ID3D12GraphicsCommandList* commandList);

	void PostDraw();

	void Initialize(const std::string& name, bool smoothing = false);

private:


	/// <summary>
	/// モデル読み込み
	/// </summary>
	/// <param name="modelName"></param>
	/// <param name="smoothing"></param>
	void LoadModel(const std::string& name, bool smoothing);

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

private: // 設定・取得
	// デスクリプタサイズ
	static UINT sDescriptorHandleIncrementSize_;
	// コマンドリスト
	static ID3D12GraphicsCommandList* sCommandList_;
	// ルートシグネチャ
	static Microsoft::WRL::ComPtr<ID3D12RootSignature> sRootSignature_;
	// パイプラインステートオブジェ
	static std::array<Microsoft::WRL::ComPtr<ID3D12PipelineState>,
		size_t(Pipeline::BlendMode::kCountOfBlendMode)> sPipelineStates_;


private:
	Model* model_;
	WorldTransform worldTransform_;

	uint32_t texture_ = 0u;
private:
	// 名前
	std::string name_;
	// メッシュ
	std::vector<Mesh*> meshes_;
	// マテリアルコンテナ
	std::unordered_map<std::string, Material*> materials_;
	// デフォルトマテリアル
	Material* defaultMaterial_ = nullptr;
	// ブレンド
	Pipeline::BlendMode blendMode_ = Pipeline::BlendMode::kNormal;
};


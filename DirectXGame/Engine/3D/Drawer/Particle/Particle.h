#pragma once
#include "StructManager.h"
#include "Model.h"
#include <memory>

class Particle
{
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


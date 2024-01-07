#pragma once
#include "StructManager.h"
#include "Model.h"
#include "CBuffer.h"
#include "ParticleData.h"
#include <memory>
#include <list>

class Particle
{
private:
	static const std::string kBaseDirectory;
	static const std::string kDefaultName;

public:
	static Particle* GetInstance() {
		static Particle instance;
		return &instance;
	}

	void StaticInitialize();

	void SRVCreate();

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

	static void PreDraw(ID3D12GraphicsCommandList* commandList);

	static void PostDraw();

	void Initialize(const std::string& name, bool smoothing = false);

	void Update();

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
	
	static const int kInstanceNum_ = 1000;

	CBufferDataParitcle gTransformationMatrices[100];

	uint32_t texture_ = 0u;

	Microsoft::WRL::ComPtr<ID3D12Resource> constBuff_;
	ConstBufferDataWorldTransform* constMap = nullptr;

	Matrix4x4 matWorld_ = {};

	void CreateConstBuffer();
	void Map();
	void TransferMatrix();
	//ConstBufferDataWorldTransform

	D3D12_CPU_DESCRIPTOR_HANDLE SrvHandleCPU;
	D3D12_GPU_DESCRIPTOR_HANDLE SrvHandleGPU;
private:

	/// メモ書き
	// パーティクルの描画はこのクラスのDrawで全てまとめて管理する予定。
	// 現在このクラスはModelと同じ機能しかないのでまずはWorldTransformの役割を
	// 配列で行うように変更していく。（この処理がParticleDataのリストを使うやつ
	// 上記の管理の体制が整い次第描画処理の部分を変更していく必要があり。
	// 描画は01_00の資料にあるやつ
	// ルートシグネチャにインスタンシング用のものを設定するところでエラーあり

	// これらを管理するリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> instancingResource_;
	//	Shaderに送る情報（下のリストをここに設定し、HLSLファイルで計算
	CBufferDataParitcle* instancingData_ = nullptr;

	// パーティクルそれぞれの座標などの情報
	std::list<ParticleData> particles_;

	int instanceCount_ = 0;

public:
	void SetMatrix(Matrix4x4 matWolrd) { matWorld_ = matWolrd; }

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


#pragma once
#include "StructManager.h"
#include "Material.h"

#include <d3d12.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <wrl.h>

class Mesh
{
public:

	struct VertexPosNormalUv {
		Vector3 pos;
		Vector3 normal;
		Vector2 uv;
	};

public:

	/// <summary>
	/// 名前のゲッター
	/// </summary>
	/// <returns></returns>
	const std::string& GetName() { return name_; }

	/// <summary>
	/// 名前をセット
	/// </summary>
	/// <param name="name"></param>
	void SetName(const std::string& name) { this->name_ = name; }

	/// <summary>
	/// 頂点の追加
	/// </summary>
	/// <param name="vertex"></param>
	void AddVertex(const VertexPosNormalUv& vertex);

	/// <summary>
	/// インデックス追加
	/// </summary>
	/// <param name="index"></param>
	void AddIndex(unsigned short index);

	/// <summary>
	/// 頂点データの数
	/// </summary>
	/// <returns></returns>
	inline size_t GetVertexCount() { return vertices_.size(); }

	/// <summary>
	/// エッジの平滑化データの追加
	/// </summary>
	/// <param name="indexPosition"></param>
	/// <param name="indexVertex"></param>
	void AddSmoothData(unsigned short indexPosition, unsigned short indexVertex);

	/// <summary>
	/// 平滑化された頂点法線の計算
	/// </summary>
	void CalculateSmoothedVertexNormals();

	/// <summary>
	/// マテリアルの取得
	/// </summary>
	/// <returns></returns>
	Material* GetMaterial() { return material_; }

	/// <summary>
	/// マテリアルのセット
	/// </summary>
	/// <param name="material"></param>
	void SetMaterial(Material* material) { this->material_ = material; }

	/// <summary>
	/// バッファ作成
	/// </summary>
	void CreateBuffers();

	/// <summary>
	/// 頂点バッファ
	/// </summary>
	/// <returns></returns>
	const D3D12_VERTEX_BUFFER_VIEW& GetVBView() { return vbView_; }

	/// <summary>
	/// インデックスバッファ
	/// </summary>
	/// <returns></returns>
	const D3D12_INDEX_BUFFER_VIEW& GetIBView() { return ibView_; }

	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name="commandList"></param>
	/// <param name="rootParameterIndexMaterial"></param>
	/// <param name="rootParameterIndexTexture"></param>
	void Draw(ID3D12GraphicsCommandList* commandList, UINT rootParameterIndexMaterial,
		UINT rootParameterIndexTexture);

	void Draw(ID3D12GraphicsCommandList* commandList, UINT rootParameterIndexMaterial,
		UINT rootParameterIndexTexture, int32_t textureHandle);


	/// <summary>
	/// パーティクル用
	/// </summary>
	void DrawInstancing(ID3D12GraphicsCommandList* commandList, UINT rootParameterIndexMaterial,
		UINT rootParameterIndexTexture, int32_t instanceCount);

	void DrawInstancing(ID3D12GraphicsCommandList* commandList, UINT rootParameterIndexMaterial,
		UINT rootParameterIndexTexture, int32_t textureHandle, int32_t instanceCount);

	/// <summary>
	/// 頂点配列
	/// </summary>
	/// <returns></returns>
	inline const std::vector<VertexPosNormalUv>& GetVertices() { return vertices_; }

	/// <summary>
	/// インデックス配列
	/// </summary>
	/// <returns></returns>
	inline const std::vector<unsigned short>& GetIndices() { return indices_; }

private:
	// 名前
	std::string name_;

	// 頂点バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff_;
	// インデックスバッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> indexBuff_;
	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView_ = {};
	// インデックスバッファビュー
	D3D12_INDEX_BUFFER_VIEW ibView_ = {};
	// 頂点データ配列
	std::vector<VertexPosNormalUv> vertices_;
	// 頂点インデックス配列
	std::vector<unsigned short> indices_;
	// 法線スムージングデータ
	std::unordered_map<unsigned short, std::vector<unsigned short>> smoothData_;
	// マテリアル
	Material* material_ = nullptr;
};


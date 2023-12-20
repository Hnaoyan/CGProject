#include "Model.h"
#include "DirectXCommon.h"
#include "D3D12Lib.h"
#include "StringManager.h"
#include "Graphics/PipelineManager.h"
#include "Graphics/Shader.h"

#include <string>
#include <cassert>
#include <fstream>
#include <sstream>

#pragma comment(lib, "d3dcompiler.lib")

using namespace Microsoft::WRL;
using namespace Pipeline;

/// <summary>
/// 静的変数の実体化
/// </summary>
const std::string Model::kBaseDirectory = "Resources/";
const std::string Model::kDefaultName = "cube";
UINT Model::sDescriptorHandleIncrementSize_ = 0;
ID3D12GraphicsCommandList* Model::sCommandList_ = nullptr;
ComPtr<ID3D12RootSignature> Model::sRootSignature_;
std::array<ComPtr<ID3D12PipelineState>, size_t(BlendMode::kCountOfBlendMode)> Model::sPipelineStates_;
std::unique_ptr<LightGroup> Model::lightGroup_;

void Model::StaticInitialize()
{
	// ライト生成
	lightGroup_.reset(LightGroup::Create());
}

Model* Model::Create()
{
	// メモリ確保
	Model* instance = new Model;
	instance->Initialize(kDefaultName, false);
	return instance;
}

Model* Model::CreatePlane()
{
	// メモリ確保
	Model* instance = new Model;
	std::string planeName = "plane";
	instance->Initialize(planeName, false);
	return instance;
}

Model* Model::CreateFromObj(const std::string& modelName, bool smoothing)
{
	// メモリ
	Model* instance = new Model;
	instance->Initialize(modelName, smoothing);

	return instance;
}

void Model::PreDraw(ID3D12GraphicsCommandList* commandList)
{
	// ペアでチェック
	assert(Model::sCommandList_ == nullptr);

	// コマンドリストをセット
	sCommandList_ = commandList;

	// ルートシグネチャの設定
	commandList->SetGraphicsRootSignature(PipelineManager::GetInstance()->sRootSignature_.Get());
	
	// プリミティブ形状を設定
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Model::Draw(const WorldTransform& worldTransform, const ViewProjection& viewProjection)
{
	// アルファ値の更新
	Update();
	// パイプラインステートの設定
	sCommandList_->SetPipelineState(PipelineManager::GetInstance()->sPipelineStates_[size_t(blendMode_)].Get());
	// ライト
	lightGroup_->Draw(sCommandList_, static_cast<UINT>(RootParameter::kLight));

	// CBV（ワールド行列）
	sCommandList_->SetGraphicsRootConstantBufferView(
		static_cast<UINT>(RootParameter::kWorldTransform),
		worldTransform.constBuff_->GetGPUVirtualAddress());

	// CBV（ビュープロジェクション行列）
	sCommandList_->SetGraphicsRootConstantBufferView(
		static_cast<UINT>(RootParameter::kViewProjection),
		viewProjection.constBuff_->GetGPUVirtualAddress());

	// 全メッシュを描画
	for (auto& mesh : meshes_) {
		mesh->Draw(
			sCommandList_, (UINT)RootParameter::kMaterial,
			(UINT)RootParameter::kTexture);
	}
}

void Model::Draw(const WorldTransform& worldTransform, const ViewProjection& viewProjection, UINT textureHandle)
{
	// アルファ値の更新
	Update();
	// パイプラインステートの設定
	sCommandList_->SetPipelineState(PipelineManager::GetInstance()->sPipelineStates_[size_t(blendMode_)].Get());
	// ライト
	lightGroup_->Draw(sCommandList_, static_cast<UINT>(RootParameter::kLight));

	// CBV（ワールド行列）
	sCommandList_->SetGraphicsRootConstantBufferView(static_cast<UINT>(RootParameter::kWorldTransform),
		worldTransform.constBuff_->GetGPUVirtualAddress());

	// CBV（ビュープロジェクション行列）
	sCommandList_->SetGraphicsRootConstantBufferView(static_cast<UINT>(RootParameter::kViewProjection),
		viewProjection.constBuff_->GetGPUVirtualAddress());
	// 全メッシュを描画
	for (auto& mesh : meshes_) {
		mesh->Draw(
			sCommandList_, (UINT)RootParameter::kMaterial,
			(UINT)RootParameter::kTexture, textureHandle);
	}
}

void Model::PostDraw()
{
	// コマンドリストの解除
	sCommandList_ = nullptr;
}

Model::~Model()
{
	for (auto m : meshes_) {
		delete m;
	}
	meshes_.clear();

	for (auto m : materials_) {
		delete m.second;
	}
	materials_.clear();

}

void Model::Initialize(const std::string& modelName, bool smoothing)
{
	// モデル読み込み
	LoadModel(modelName, smoothing);

	// メッシュ
	for (auto& m : meshes_) {
		if (m->GetMaterial() == nullptr) {
			if (defaultMaterial_ == nullptr) {
				// デフォルトマテリアルを生成
				defaultMaterial_ = Material::Create();
				defaultMaterial_->name_ = "no material";
				materials_.emplace(defaultMaterial_->name_, defaultMaterial_);
			}

			m->SetMaterial(defaultMaterial_);
		}
	}

	// メッシュのバッファ生成
	for (auto& m : meshes_) {
		m->CreateBuffers();
	}

	// マテリアルの数値を定数バッファに反映
	for (auto& m : materials_) {
		m.second->Update();
	}

	// テクスチャの読み込み
	LoadTextures();
}

void Model::Update()
{
	// マテリアルの数値を定数バッファに反映
	for (auto& m : materials_) {
		//m.second->SetAlpha(alphaValue_);
		m.second->Update();
	}
}

void Model::SetAlphaValue(float alpha) {
	// マテリアルの数値を定数バッファに反映
	for (auto& m : materials_) {
		m.second->SetAlpha(alpha);
		m.second->Update();
	}
}

float Model::GetAlphaValue() {
	float result = 0;
	for (auto& m : materials_) {
		result = m.second->GetAlpha();
	}
	return result;
}

void Model::SetMaterial(Material* material)
{
	materials_.emplace(material->name_, material);
	for (auto& m : meshes_) {
		m->SetMaterial(material);
	}

	// メッシュのバッファ生成
	for (auto& m : meshes_) {
		m->CreateBuffers();
	}

	// マテリアルの数値を定数バッファに反映
	for (auto& m : materials_) {
		m.second->Update();
	}

	// テクスチャの読み込み
	LoadTextures();

}

void Model::LoadModel(const std::string& modelName, bool smoothing)
{
	const std::string fileName = modelName + ".obj";
	const std::string directoryPath = kBaseDirectory + modelName + "/";

	// ファイルストリーム
	std::ifstream file;
	// .objファイルを開く
	file.open(directoryPath + fileName);
	// 失敗をチェック
	if (file.fail()) {
		assert(0);
	}

	name_ = modelName;

	// メッシュ生成
	meshes_.emplace_back(new Mesh);
	Mesh* mesh = meshes_.back();
	int indexCountTex = 0;
	//int indexCountNoTex = 0;

	std::vector<Vector3> positions;
	std::vector<Vector3> normals;
	std::vector<Vector2> texcoords;

	std::string line;
	while (std::getline(file, line)) {

		// 1行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		// 先頭の識別子確認
		std::string key;
		std::getline(line_stream, key, ' ');

		// マテリアル
		if (key == "mtllib") {
			// マテリアルファイル名読み込み
			std::string filename;
			line_stream >> filename;
			// マテリアル読み込み
			LoadMaterial(directoryPath, filename);
		}
		// 先頭文字列がgならグループの開始
		if (key == "g") {

			// カレントメッシュの情報が揃っているなら
			if (mesh->GetName().size() > 0 && mesh->GetVertexCount() > 0) {
				// エッジの平滑化
				if (smoothing) {
					mesh->CalculateSmoothedVertexNormals();
				}
				// メッシュの生成
				meshes_.emplace_back(new Mesh);
				mesh = meshes_.back();
				indexCountTex = 0;
			}

			// グループ名読み込み
			std::string groupName;
			line_stream >> groupName;

			// メッシュに名前をセット
			mesh->SetName(groupName);
		}
		// 先頭文字がvなら頂点座標
		if (key == "v") {
			// X,Y,Z座標読み込み
			Vector3 position;
			line_stream >> position.x;
			line_stream >> position.y;
			line_stream >> position.z;
			// 頂点座標データに追加
			positions.emplace_back(position);
		}
		// 先頭文字列がvtならテクスチャ
		if (key == "vt") {
			// UV成分読み込み
			Vector2 texcoord;
			line_stream >> texcoord.x >> texcoord.y;

			// V方向反転
			texcoord.y = 1.0f - texcoord.y;

			// テクスチャ座標データに追加
			texcoords.emplace_back(texcoord);
		}
		// 先頭文字列がvnなら法線ベクトル
		if (key == "vn") {
			// XYZ成分
			Vector3 normal;
			line_stream >> normal.x >> normal.y >> normal.z;
			// 法線ベクトルデータに追加
			normals.emplace_back(normal);
		}
		// 先頭文字列がusemtlならマテリアルを割り当てる
		if (key == "usemtl") {
			if (mesh->GetMaterial() == nullptr) {
				// マテリアルの名読み込み
				std::string materialName;
				line_stream >> materialName;

				// マテリアル名で検索し、マテリアルを割り当てる
				auto itr = materials_.find(materialName);
				if (itr != materials_.end()) {
					mesh->SetMaterial(itr->second);
				}

			}
		}
		// 先頭文字列がfならポリゴン（三角形）
		if (key == "f") {
			int faceIndexCount = 0;
			// 半角スペース区切りで行の続きを読み込む
			std::string index_string;

			while (std::getline(line_stream, index_string, ' ')) {
				// 頂点インデックス一個分の文字列をストリームに変換して解析しやすくする
				std::istringstream index_stream(index_string);
				unsigned short indexPosition, indexTexcoord, indexNormal;
				// 頂点番号
				index_stream >> indexPosition;

				Material* material = mesh->GetMaterial();
				index_stream.seekg(1, std::ios_base::cur);
				// マテリアル、テクスチャがある場合
				if (material && material->textureFilename_.size() > 0) {
					index_stream >> indexTexcoord;
					index_stream.seekg(1, std::ios_base::cur);
					index_stream >> indexNormal;
					// 頂点データの追加
					Mesh::VertexPosNormalUv vertex{};
					vertex.pos = positions[indexPosition - 1];
					vertex.normal = normals[indexNormal - 1];
					vertex.uv = texcoords[indexTexcoord - 1];
					mesh->AddVertex(vertex);

					if (smoothing) {
						mesh->AddSmoothData(
							indexPosition, (unsigned short)mesh->GetVertexCount() - 1);
					}
				}
				else {
					char c;
					index_stream >> c;
					// スラッシュ2連続の場合、頂点番号のみ
					if (c == '/') {
						// 頂点データの追加
						Mesh::VertexPosNormalUv vertex{};
						vertex.pos = positions[indexPosition - 1];
						vertex.normal = { 0,0,1 };
						vertex.uv = { 0,0 };
						mesh->AddVertex(vertex);

					}
					else {
						index_stream.seekg(-1, std::ios_base::cur);
						index_stream >> indexTexcoord;
						index_stream.seekg(1, std::ios_base::cur);
						index_stream >> indexNormal;
						// 頂点データの追加
						Mesh::VertexPosNormalUv vertex{};
						vertex.pos = positions[indexPosition - 1];
						vertex.normal = normals[indexNormal - 1];
						vertex.uv = { 0,0 };
						mesh->AddVertex(vertex);

						if (smoothing) {
							mesh->AddSmoothData(
								indexPosition, (unsigned short)mesh->GetVertexCount() - 1);
						}
					}
				}
				// インデックスデータの追加
				if (faceIndexCount >= 3) {
					// 四角形のポリゴン4点目なので
					// 四角形の0,1,2,3の内 2,3,0で三角形を構築
					mesh->AddIndex(static_cast<unsigned short>(indexCountTex - 1));
					mesh->AddIndex(static_cast<unsigned short>(indexCountTex));
					mesh->AddIndex(static_cast<unsigned short>(indexCountTex - 3));

				}
				else {
					mesh->AddIndex(static_cast<unsigned short>(indexCountTex));
				}
				indexCountTex++;
				faceIndexCount++;
			}


		}
	}

	file.close();

	if (smoothing) {
		mesh->CalculateSmoothedVertexNormals();
	}

}

void Model::LoadMaterial(const std::string& directoryPath, const std::string& fileName)
{
	// ファイルストリーム
	std::ifstream file;

	file.open(directoryPath + fileName);
	// ファイルオープンチェック
	if (file.fail()) {
		assert(0);
	}

	Material* material = nullptr;

	// 1行ずつ読み込む
	std::string line;
	while (std::getline(file, line))
	{

		// 1行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		// 半角スペース区切りで行の先頭文字列を取得
		std::string key;
		std::getline(line_stream, key, ' ');

		if (key[0] == '\t') {
			key.erase(key.begin());
		}

		if (key == "newmtl") {

			// 既にマテリアルがあれば
			if (material) {
				// マテリアルをコンテナに登録
				AddMaterial(material);
			}

			// 新しいマテリアルを生成
			material = Material::Create();
			// マテリアル名読み込み
			line_stream >> material->name_;

		}
		// 先頭文字列がKaならアンビエント色
		if (key == "Ka") {
			line_stream >> material->ambient_.x >> material->ambient_.y >> material->ambient_.z;
		}
		// 先頭文字列がKdならディフューズ色
		if (key == "Kd") {
			line_stream >> material->diffuse_.x >> material->diffuse_.y >> material->diffuse_.z;
		}
		if (key == "Ks") {
			line_stream >> material->specular_.x >> material->specular_.y >> material->specular_.z;
		}
		// 先頭文字列がmap_Kdならテクスチャファイル名
		if (key == "map_Kd") {
			// テクスチャのファイル名読み込み
			line_stream >> material->textureFilename_;

			// テクスチャのファイル名を取り出す
			size_t pos1;
			pos1 = material->textureFilename_.rfind('\\');
			if (pos1 != std::string::npos) {
				material->textureFilename_ = material->textureFilename_.substr(
					pos1 + 1, material->textureFilename_.size() - pos1 - 1);
			}

			pos1 = material->textureFilename_.rfind('/');
			if (pos1 != std::string::npos) {
				material->textureFilename_ = material->textureFilename_.substr(
					pos1 + 1, material->textureFilename_.size() - pos1 - 1);
			}

		}

	}
	// ファイルを閉じる
	file.close();

	if (material) {
		AddMaterial(material);
	}

}

void Model::AddMaterial(Material* material)
{
	// コンテナ登録
	materials_.emplace(material->name_, material);
}

void Model::LoadTextures()
{
	int textureIndex = 0;
	std::string directoryPath = name_ + "/";

	for (auto& m : materials_) {
		Material* material = m.second;

		// テクスチャあり
		if (material->textureFilename_.size() > 0) {
			// テクスチャ読み込み
			material->LoadTexture(directoryPath);
			textureIndex++;
		}
		// テクスチャなし
		else
		{
			// 仮のテクスチャ読み込み
			material->LoadTexture("white1x1.png");
			textureIndex++;
		}
	}
}

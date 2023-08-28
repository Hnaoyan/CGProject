#include "Model.h"
#include "DirectXCommon.h"
#include "D3D12Lib.h"
#include "StringManager.h"

#include <string>
#include <cassert>
#include <fstream>
#include <sstream>

#pragma comment(lib, "d3dcompiler.lib")

using namespace Microsoft::WRL;

/// <summary>
/// 静的変数の実体化
/// </summary>
const std::string Model::kBaseDirectory = "resources/";
const std::string Model::kDefaultName = "cube";
UINT Model::sDescriptorHandleIncrementSize_ = 0;
ID3D12GraphicsCommandList* Model::sCommandList_ = nullptr;
ComPtr<ID3D12RootSignature> Model::sRootSignature_;
ComPtr<ID3D12PipelineState> Model::sPipelineState_;
std::unique_ptr<LightGroup> Model::lightGroup_;

void Model::StaticInitialize()
{

	// パイプライン初期化
	InitializeGraphicsPipeline();

	// ライト生成
	lightGroup_.reset(LightGroup::Create());

}

void Model::InitializeGraphicsPipeline()
{
	HRESULT result = S_FALSE;
	ComPtr<IDxcBlob> vsBlob;
	ComPtr<IDxcBlob> psBlob;
	ComPtr<ID3DBlob> errorBlob;
	ComPtr<ID3DBlob> rootSigBlob;

	// 頂点シェーダの読み込みとコンパイル
	vsBlob = D3D12Lib::GetInstance()->CompileShader(L"resources/shaders/ObjVS.hlsl",
			L"vs_6_0");
	assert(vsBlob != nullptr);

	// ピクセルシェーダの読み込みとコンパイル
	psBlob = D3D12Lib::GetInstance()->CompileShader(L"resources/shaders/ObjPS.hlsl",
		L"ps_6_0");
	assert(psBlob != nullptr);

	// 頂点レイアウト
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{
			"POSITION",	0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		},
		{
			"NORMAL",	0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		},
		{
			"TEXCOORD",	0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		},
	};

	// グラフィックスパイプライン
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
	gpipeline.VS = D3D12Lib::ShaderByteCode(vsBlob.Get());
	gpipeline.PS = D3D12Lib::ShaderByteCode(psBlob.Get());

	// サンプルマスク
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
	// ラスタライザステート
	gpipeline.RasterizerState = D3D12Lib::SetRasterizer();
	// デプスステンシルステート
	gpipeline.DepthStencilState.DepthEnable = true;
	gpipeline.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	gpipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;

	// レンダーターゲットのブレンド設定
	D3D12_BLEND_DESC blenddesc{};
	blenddesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	// ブレンドステート
	gpipeline.BlendState = blenddesc;

	// 深度バッファのフォーマット
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	// 頂点レイアウト
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	// 図形の形状設定
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	gpipeline.NumRenderTargets = 1;
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	gpipeline.SampleDesc.Count = 1;

	// デスクリプタレンジ
	D3D12_DESCRIPTOR_RANGE descRangeSRV;
	descRangeSRV = D3D12Lib::Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);

	// ルートパラメータ
	D3D12_ROOT_PARAMETER rootparams[5];
	rootparams[0] = D3D12Lib::InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[1] = D3D12Lib::InitAsConstantBufferView(1, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[2] = D3D12Lib::InitAsConstantBufferView(2, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[3] = D3D12Lib::InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[4] = D3D12Lib::InitAsConstantBufferView(3, 0, D3D12_SHADER_VISIBILITY_ALL);

	// スタティックサンプラー
	D3D12_STATIC_SAMPLER_DESC samplerDesc[1];
	samplerDesc[0] = D3D12Lib::SetSamplerDesc(0, D3D12_FILTER_MIN_MAG_MIP_LINEAR);
	samplerDesc[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	samplerDesc[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	samplerDesc[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;


	// ルートシグネチャの設定
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	rootSignatureDesc.pParameters = rootparams;
	rootSignatureDesc.NumParameters = _countof(rootparams);

	rootSignatureDesc.pStaticSamplers = samplerDesc;
	rootSignatureDesc.NumStaticSamplers = _countof(samplerDesc);

	// バージョン自動判定のシリアライズ
	result = D3D12SerializeRootSignature(
		&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
	if (FAILED(result)) {
		Log(reinterpret_cast<char*>(errorBlob->GetBufferPointer()));
		assert(false);
	}

	// ルートシグネチャの生成
	result = DirectXCommon::GetInstance()->GetDevice()->CreateRootSignature(
		0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&sRootSignature_));
	assert(SUCCEEDED(result));

	gpipeline.pRootSignature = sRootSignature_.Get();

	// グラフィックスパイプラインの生成
	result = DirectXCommon::GetInstance()->GetDevice()->CreateGraphicsPipelineState(
		&gpipeline, IID_PPV_ARGS(&sPipelineState_));
	assert(SUCCEEDED(result));

}

Model* Model::Create()
{
	// メモリ確保
	Model* instance = new Model;
	instance->Initialize(kDefaultName, false);
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

	// パイプラインステートの設定
	commandList->SetPipelineState(sPipelineState_.Get());
	// ルートシグネチャの設定
	commandList->SetGraphicsRootSignature(sRootSignature_.Get());
	// プリミティブ形状を設定
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
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
	while (std::getline(file,line))
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

void Model::Draw(const WorldTransform& worldTransform, const ViewProjection& viewProjection)
{
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

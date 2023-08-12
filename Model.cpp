#include "Model.h"
#include "D3D12Lib/D3D12Lib.h"

#include <string>
#include <cassert>
#include <fstream>
#include <sstream>

#pragma comment(lib, "d3dcompiler.lib")

using namespace Microsoft::WRL;
const std::string Model::kBaseDirectory = "resources/";
const std::string Model::kDefaultName = "cube";

ID3D12GraphicsCommandList* Model::sCommandList_ = nullptr;
ComPtr<ID3D12RootSignature> Model::sRootSignature_;
ComPtr<ID3D12PipelineState> Model::sPipelineState_;

void Model::StaticInitialize()
{

	// パイプライン初期化
	InitializeGraphicsPipeline();

}

void Model::InitializeGraphicsPipeline()
{
	HRESULT result = S_FALSE;
	ComPtr<IDxcBlob> vsBlob;
	ComPtr<IDxcBlob> psBlob;
	ComPtr<ID3DBlob> errorBlob;

	ComPtr<IDxcUtils> dxcUtils_;
	ComPtr<IDxcCompiler3> dxcCompiler_;
	ComPtr<IDxcIncludeHandler> includeHandler_;

	// 頂点シェーダの読み込みとコンパイル
	vsBlob = D3D12Lib::CompileShader(L"resources/shaders/Sprite.VS.hlsl",
			L"vs_6_0", dxcUtils_.Get(), dxcCompiler_.Get(), includeHandler_.Get());
	assert(vsBlob != nullptr);

	// ピクセルシェーダの読み込みとコンパイル
	psBlob = D3D12Lib::CompileShader(L"resources/shaders/Sprite.PS.hlsl",
		L"ps_6_0", dxcUtils_.Get(), dxcCompiler_.Get(), includeHandler_.Get());
	assert(psBlob != nullptr);

	// 頂点レイアウト
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{
			"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		},
		{
			"NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		},
		{
			"TEXCOORD",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,
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
	gpipeline.DepthStencilState;

	// レンダーターゲットのブレンド設定
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	blenddesc.BlendEnable = true;
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;

	// ブレンドステート
	gpipeline.BlendState.RenderTarget[0] = blenddesc;

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
	commandList->SetComputeRootSignature(sRootSignature_.Get());
	// プリミティブ形状を設定
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Model::PostDraw()
{
	// コマンドリストの解除
	sCommandList_ = nullptr;
}

void Model::Initialize(const std::string& modelName, bool smoothing)
{
	// モデル読み込み
	LoadModel(modelName, smoothing);

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
		assert(file.is_open());
	}

	ModelData modelData;
	std::vector<Vector4> positions;
	std::vector<Vector3> normals;
	std::vector<Vector2> texcoords;
	std::string line;

	while (std::getline(file, line)) {

		// 1行分の文字列をストリームに変換して解析しやすくする
		std::istringstream s(line);

		// 先頭の識別子確認
		std::string key;
		std::getline(s, key, ' ');

		// 先頭文字がvなら頂点座標
		if (key == "v") {
			Vector4 position;
			s >> position.x >> position.y >> position.z;
			position.w = 1.0f;
			// 頂点座標データに追加
			positions.push_back(position);
		}
		// 先頭文字列がvtならテクスチャ
		if (key == "vt") {
			// UV成分読み込み
			Vector2 texcoord;
			s >> texcoord.x >> texcoord.y;
			// テクスチャ座標データに追加
			texcoords.push_back(texcoord);
		}
		// 先頭文字列がvnなら法線ベクトル
		if (key == "vn") {
			// XYZ成分
			Vector3 normal;
			s >> normal.x >> normal.y >> normal.z;
			// 法線ベクトルデータに追加
			normals.push_back(normal);
		}
		// 先頭文字列がfならポリゴン（三角形）
		if (key == "f") {
			int faceIndexCount = 0;
			// 半角スペース区切りで行の続きを読み込む
			std::string index_string;


		}
	}
}

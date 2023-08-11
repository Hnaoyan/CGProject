#pragma once
#include "StructManager.h"
#include <vector>
#include <string>
#include <wrl.h>

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

	struct ModelData {
		//std::vector<>
	};


private:	// 静的メンバ変数
	// デスクリプタサイズ
	static UINT sDescriptorHandleIncrementSize_;
	// コマンドリスト
	static ID3D12GraphicsCommandList* sCommandList_;
	// ルートシグネチャ
	static Microsoft::WRL::ComPtr<ID3D12RootSignature> sRootSignature_;
	// パイプラインステートオブジェ
	static Microsoft::WRL::ComPtr<ID3D12PipelineState> sPipelineState_;

public:

	static void StaticInitialize();

	static void InitializeGraphicsPipeline();

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

};


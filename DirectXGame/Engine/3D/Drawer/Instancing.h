#pragma once
#include "StructManager.h"
#include "DirectXCommon.h"
#include <memory>

struct VertexDataInst {
	Vector4 position;
	Vector2 texcoord;
	Vector3 normal;
};

struct MaterialData {
	std::string textureFilePath;
};

struct ModelData {
	std::vector<VertexDataInst> vertices;
	MaterialData material;
};

class Instancing
{
private:

	struct MaterialInfo {
		Vector4 color;
		int32_t enableLighting;
	};

	struct Transform {
		Vector3 translate;
		Vector3 scale;
		Vector3 rotate;
	};

	struct TransformationMatrix
	{
		Matrix4x4 WVP;
		Matrix4x4 World;
	};

	struct DirectionalLight {
		Vector4 color;	// ライトの色
		Vector3 direction;	// ライトの向き
		float intensity;	// 輝度
	};
private:
	uint32_t kMaxCount_ = 20;
	uint32_t instancingCount_;

private:
	Transform transforms[20];
	Transform cameraTransform_{ {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,-10.0f} };

public:
	void Create();
	void CreateData();
	void CreateMesh();
	void CreateTexture();

	void Initialize();
	void Update();
	void UpdateMatrix();
	void ImGuiWidget();
	void Draw();

public:
	void PreDraw(ID3D12GraphicsCommandList* commandList);
	void PostDraw();

private:
	DirectXCommon* dxCommon_ = nullptr;
	ID3D12Device* device_ = nullptr;
	// コマンドリスト
	ID3D12GraphicsCommandList* sCommandList_;

private:
	ModelData LoadPlane(const std::string& directory, const std::string& fileName);
	MaterialData LoadMaterial(const std::string& directory, const std::string& fileName);

private:
	// マテリアル
	ID3D12Resource* materialResource_ = nullptr;
	MaterialInfo* materialData_ = nullptr;

	// ライト
	ID3D12Resource* directionalLightResource_ = nullptr;
	DirectionalLight* directionalLightData_ = nullptr;

	ModelData modelData_;
	VertexDataInst* vertexData_ = nullptr;

	ID3D12Resource* vertexResource_ = nullptr;
	D3D12_VERTEX_BUFFER_VIEW vbView{};

	D3D12_CPU_DESCRIPTOR_HANDLE textureSrvHandleCPU[1];
	D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU[1];

private:
	Microsoft::WRL::ComPtr<ID3D12Resource> texResource_;

	TransformationMatrix* instancingData_ = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> instancingResource_;


	D3D12_CPU_DESCRIPTOR_HANDLE instancingSrvHandleCPU;
	D3D12_GPU_DESCRIPTOR_HANDLE instancingSrvHandleGPU;

};


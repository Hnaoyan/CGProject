#pragma once
#include "StructManager.h"
#include "DirectXCommon.h"

struct VertexData {
	Vector4 position;
	Vector2 texcoord;
	Vector3 normal;
};

struct MaterialData {
	std::string textureFilePath;
};

struct ModelData {
	std::vector<VertexData> vertices;
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


public:
	void Create();
	void CreateMesh();
	void Initialize();
	void Update();
	void Draw();

public:
	void PreDraw();
	void PostDraw();

private:
	ModelData LoadPlane(const std::string& directory, const std::string& fileName);
	MaterialData LoadMaterial(const std::string& directory, const std::string& fileName);

	uint32_t kMaxCount_ = 20;
	uint32_t instancingCount_;

	ID3D12Resource* materialResource_ = nullptr;
	// 情報
	MaterialInfo* materialData_ = nullptr;

	ID3D12Resource* CreateBufferResource(ID3D12Device* device, size_t sizeInBytes);
	ID3D12Device* device_ = nullptr;
};


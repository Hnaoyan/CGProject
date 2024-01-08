#pragma once
#include "CBuffer.h"
#include "DirectXCommon.h"
#include "TextureManager.h"
#include "ViewProjection.h"

class TestParticle
{
public:
	void Initialize(uint32_t Drawparticle);
	void PreDraw(ID3D12GraphicsCommandList* commandList);
	void Draw(const ViewProjection& viewProjection, const Vector4& material, uint32_t index);
	void PostDraw() {
		sCommandList_ = nullptr;
	}
	void Update();
	void Finalize() {};
	//void AddParticle(const Emitter& emitter, const int& count);
private:
	void SettingVertex();
	void SetColor();
	void TransformMatrix();
	void CreateSRV(uint32_t num);
private:
	ID3D12GraphicsCommandList* sCommandList_ = nullptr;
	TextureManager* textureManager_;
	//SrvDescriptorHeap* srvHeap_;
	SRV* srv_;
	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource_;
	DirectXCommon* dxCommon_;
	VertexData* vertexData_;
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource_;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;

	Material_Test* materialData_;
	Microsoft::WRL::ComPtr<ID3D12Resource> wvpResource_;
	Transformmatrix* wvpData_;
	//DirectionalLight* directionalLight_;
	static	const uint32_t kNumMaxInstance_ = 100000;
	Microsoft::WRL::ComPtr<ID3D12Resource> instancingResource_;
	ParticleForGPU* instancingData;
	std::list<ParticleData_Test>particles_;
	uint32_t index_;
	int knumInstance_;
	uint32_t DrawInstanceNum_;
	int instanceCount;
private:
	//ParticleData MakeNewParticle(const Emitter& emitter, std::mt19937& randomEngine);

	ID3D12Resource* CreateBufferResource(ID3D12Device* device, size_t sizeInBytes) {
		//頂点リソース用のヒープの設定
		D3D12_HEAP_PROPERTIES uplodeHeapProperties{};
		uplodeHeapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;//UploadHeapを使う
		//頂点リソースの設定
		D3D12_RESOURCE_DESC ResourceDesc{};
		//バッファリソース。テクスチャの場合はまた別の設定をする
		ResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		ResourceDesc.Width = sizeInBytes;//リソースサイズ
		//バッファの場合はこれらは１にする決まり
		ResourceDesc.Height = 1;
		ResourceDesc.DepthOrArraySize = 1;
		ResourceDesc.MipLevels = 1;
		ResourceDesc.SampleDesc.Count = 1;
		//バッファの場合はこれにする決まり
		ResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		HRESULT hr;
		ID3D12Resource* Resource = nullptr;
		//	ID3D12Resource* Resource = nullptr;
		//実際に頂点リソースを作る
		hr = device->CreateCommittedResource(&uplodeHeapProperties, D3D12_HEAP_FLAG_NONE,
			&ResourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
			IID_PPV_ARGS(&Resource));
		assert(SUCCEEDED(hr));

		return Resource;
	}
};


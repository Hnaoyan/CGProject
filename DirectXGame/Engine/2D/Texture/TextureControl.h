//#pragma once
//#include <wrl.h>
//#include <array>
//
//#include "DirectXCommon.h"
//#include "DirectXTex.h"
//#include "d3dx12.h"
//
//
//class TextureControl
//{
//public:
//	std::string kDirectoryPath = "Resources/";
//
//	void Initialize();
//
//	static TextureControl* GetInstance() {
//		static TextureControl instance;
//		return &instance;
//	}
//	static const int maxtex = 256;
//	const D3D12_GPU_DESCRIPTOR_HANDLE GetGPUHandle(uint32_t textureHandle);
//
//	uint32_t GetSizeRTV() { return descriptorSizeRTV; }
//	uint32_t GetSizeDSV() { return descriptorSizeDSV; }
//	uint32_t Load(const std::string& filePath);
//
//private:
//	DirectXCommon* dxCommon_;
//	SRV* SrvHeap_;
//	Microsoft::WRL::ComPtr<ID3D12Resource>intermediateResource[maxtex];
//	Microsoft::WRL::ComPtr<ID3D12Resource>textureResource[maxtex];
//
//	uint32_t descriptorSizeRTV;
//	uint32_t descriptorSizeDSV;
//	std::vector<std::string> name_;
//private:
//	ID3D12Resource* CreateTextureResource(ID3D12Device* device, const DirectX::TexMetadata& metadata);
//
//	Microsoft::WRL::ComPtr<ID3D12Resource> UploadtextureData(ID3D12Resource* texture, const DirectX::ScratchImage& mipImages, uint32_t index);
//
//	DirectX::ScratchImage  LoadTexture(const std::string& filePath);
//
//	uint32_t LoadTexture(const std::string& filePath, uint32_t index);
//
//
//	ID3D12Resource* CreateBufferResource(ID3D12Device* device, size_t sizeInBytes) {
//		//頂点リソース用のヒープの設定
//		D3D12_HEAP_PROPERTIES uplodeHeapProperties{};
//		uplodeHeapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;//UploadHeapを使う
//		//頂点リソースの設定
//		D3D12_RESOURCE_DESC ResourceDesc{};
//		//バッファリソース。テクスチャの場合はまた別の設定をする
//		ResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
//		ResourceDesc.Width = sizeInBytes;//リソースサイズ
//		//バッファの場合はこれらは１にする決まり
//		ResourceDesc.Height = 1;
//		ResourceDesc.DepthOrArraySize = 1;
//		ResourceDesc.MipLevels = 1;
//		ResourceDesc.SampleDesc.Count = 1;
//		//バッファの場合はこれにする決まり
//		ResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
//		HRESULT hr;
//		ID3D12Resource* Resource = nullptr;
//		//	ID3D12Resource* Resource = nullptr;
//		//実際に頂点リソースを作る
//		hr = device->CreateCommittedResource(&uplodeHeapProperties, D3D12_HEAP_FLAG_NONE,
//			&ResourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
//			IID_PPV_ARGS(&Resource));
//		assert(SUCCEEDED(hr));
//
//		return Resource;
//	}
//
//};
//

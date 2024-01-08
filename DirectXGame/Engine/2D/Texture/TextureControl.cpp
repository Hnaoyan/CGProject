//#include "TextureControl.h"
//#include "StringManager.h"
//
//void TextureControl::Initialize()
//{
//	dxCommon_ = DirectXCommon::GetInstance();
//	SrvHeap_ = dxCommon_->GetSRV();
//	descriptorSizeDSV = dxCommon_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
//	descriptorSizeRTV = dxCommon_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
//	SrvHeap_->Initialize();
//	name_.clear();
//
//}
//
//const D3D12_GPU_DESCRIPTOR_HANDLE TextureControl::GetGPUHandle(uint32_t textureHandle)
//{
//	D3D12_GPU_DESCRIPTOR_HANDLE GPUHandle = SrvHeap_->GetGPUHandle(textureHandle);
//	return GPUHandle;
//}
//
//uint32_t TextureControl::Load(const std::string& filePath)
//{
//	return LoadTexture(filePath, SrvHeap_->GetIndex());
//}
//
//ID3D12Resource* TextureControl::CreateTextureResource(ID3D12Device* device, const DirectX::TexMetadata& metadata)
//{
//	//メタデータをもとにResourceの設定
//	D3D12_RESOURCE_DESC resourceDesc{};
//	resourceDesc.Width = UINT(metadata.width);//texturの幅
//	resourceDesc.Height = UINT(metadata.height);
//	resourceDesc.MipLevels = UINT16(metadata.mipLevels);//mipmapの数
//	resourceDesc.DepthOrArraySize = UINT16(metadata.arraySize);//奥行or配列テクスチャの配列数
//	resourceDesc.Format = metadata.format;//TextureのFormat
//	resourceDesc.SampleDesc.Count = 1;//サンプリングカウント.1固定
//	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION(metadata.dimension);//テクスチャの次元数
//
//	//利用するheapの設定。非常に特殊な運用
//	D3D12_HEAP_PROPERTIES heapProperties{};
//	heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;//細かい設定を行う
//
//	//Resourceの作成
//	ID3D12Resource* resource = nullptr;
//	HRESULT hr;
//	hr = device->CreateCommittedResource(&heapProperties, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(&resource));
//	assert(SUCCEEDED(hr));
//	return resource;
//}
//
//[[nodiscard]]
//Microsoft::WRL::ComPtr<ID3D12Resource> TextureControl::UploadtextureData(ID3D12Resource* texture, const DirectX::ScratchImage& mipImages, uint32_t index) {
//	std::vector<D3D12_SUBRESOURCE_DATA>subresource;
//	DirectX::PrepareUpload(dxCommon_->GetDevice(), mipImages.GetImages(), mipImages.GetImageCount(), mipImages.GetMetadata(), subresource);
//	uint64_t  intermediateSize = GetRequiredIntermediateSize(texture, 0, UINT(subresource.size()));
//	intermediateResource[index] = CreateBufferResource(dxCommon_->GetDevice(), intermediateSize);
//	UpdateSubresources(dxCommon_->GetCommandList(), texture, intermediateResource[index].Get(), 0, 0, UINT(subresource.size()), subresource.data());
//
//	D3D12_RESOURCE_BARRIER barrier{};
//	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
//	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
//	barrier.Transition.pResource = texture;
//	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
//	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
//	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_GENERIC_READ;
//	dxCommon_->GetCommandList()->ResourceBarrier(1, &barrier);
//	return intermediateResource[index];
//
//
//}
//
//DirectX::ScratchImage TextureControl::LoadTexture(const std::string& filePath)
//{
//	//テクスチャファイルを読んでうろグラムで扱えるようにする
//	DirectX::ScratchImage image{};
//	std::wstring filePathW = ConvertString(filePath);
//	HRESULT hr = DirectX::LoadFromWICFile(filePathW.c_str(), DirectX::WIC_FLAGS_FORCE_SRGB, nullptr, image);
//	assert(SUCCEEDED(hr));
//
//	//ミップマップの作成
//	DirectX::ScratchImage mipImages{};
//	hr = DirectX::GenerateMipMaps(image.GetImages(), image.GetImageCount(), image.GetMetadata(), DirectX::TEX_FILTER_SRGB, 0, mipImages);
//	assert(SUCCEEDED(hr));
//	return mipImages;
//}
//
//uint32_t TextureControl::LoadTexture(const std::string& filePath, uint32_t index)
//{
//	if (!name_.empty()) {
//		std::vector<std::string>::iterator it = std::find_if(name_.begin(), name_.end(), [&](const auto& name) {
//			return name == filePath;
//			});
//		if (it != name_.end()) {
//			return uint32_t(std::distance(name_.begin(), it)) + 2;
//		}
//	}
//	std::string fullPath = kDirectoryPath + filePath;
//	name_.push_back(fullPath);
//	DirectX::ScratchImage mipImage = LoadTexture(fullPath);
//	const DirectX::TexMetadata& metadata = mipImage.GetMetadata();
//	textureResource[index] = CreateTextureResource(dxCommon_->GetDevice(), metadata);
//	textureResource[index] = UploadtextureData(textureResource[index].Get(), mipImage, index);
//	//metaDataを元にSRVの設定
//	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
//	srvDesc.Format = metadata.format;
//	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
//	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2Dテクスチャ
//	srvDesc.Texture2D.MipLevels = UINT(metadata.mipLevels);
//
//	//SRVを作成するDescripterHeapの場所を決める
//	SrvHeap_->SetCPUHandle(SrvHeap_->GetHeap(), SrvHeap_->GetSizeSRV(), index);//direct_->GetSrvHeap()->GetGPUDescriptorHandleForHeapStart();
//	SrvHeap_->SetGPUHandle(SrvHeap_->GetHeap(), SrvHeap_->GetSizeSRV(), index);
//	//先頭はIMGUIが使ってるからその次を使う
//	//textureSrvHandleCPU_[index].ptr += dirctXCommon_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
//	//textureSrvHandleGPU_[index].ptr += dirctXCommon_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
//	SrvHeap_->AddPtr(index, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
//
//	dxCommon_->GetDevice()->CreateShaderResourceView(textureResource[index].Get(), &srvDesc, SrvHeap_->GetCPUHandle(index));
//	SrvHeap_->AddIndex();
//	return SrvHeap_->GetIndex() - 1;
//}

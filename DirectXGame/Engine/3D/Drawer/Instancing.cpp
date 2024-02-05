#include "Instancing.h"
#include "Graphics/PipelineManager.h"
#include "StringManager.h"
#include "MathLib.h"
#include "imgui.h"

#include <fstream>
#include <sstream>
#include <cassert>

DirectX::ScratchImage LoadTexture(const std::string& filePath)
{
	// テクスチャファイルを読んでプログラムで扱えるようにする
	DirectX::ScratchImage image{};
	std::wstring filePathW = ConvertString(filePath);
	HRESULT hr;
	hr = DirectX::LoadFromWICFile(filePathW.c_str(), DirectX::WIC_FLAGS_FORCE_SRGB, nullptr, image);
	assert(SUCCEEDED(hr));

	// ミップマップの作成
	DirectX::ScratchImage mipImages{};
	hr = DirectX::GenerateMipMaps(image.GetImages(), image.GetImageCount(), image.GetMetadata(), DirectX::TEX_FILTER_SRGB, 0, mipImages);
	assert(SUCCEEDED(hr));

	// ミップマップ付きのデータを返す
	return mipImages;
}

ID3D12Resource* CreateBufferResource(ID3D12Device* device, size_t sizeInBytes) {
	IDXGIFactory7* dxgiFactory = nullptr;
	HRESULT hr;
	hr = CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory));
	// 頂点リソース用のヒープの設定
	D3D12_HEAP_PROPERTIES uploadHeapProperties{};
	uploadHeapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;	// UploadHeapを使う
	// 頂点リソースの設定
	D3D12_RESOURCE_DESC resourceDesc{};
	// バッファリソース。テクスチャの場合はまた別の設定をする
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resourceDesc.Width = sizeInBytes;	// リソースのサイズ
	// バッファの場合はこれらは1にする決まり
	resourceDesc.Height = 1;
	resourceDesc.DepthOrArraySize = 1;
	resourceDesc.MipLevels = 1;
	resourceDesc.SampleDesc.Count = 1;
	// バッファの場合はこれにする決まり
	resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	// 実際に頂点リソースを作る
	ID3D12Resource* resultResource = nullptr;
	hr = device->CreateCommittedResource(&uploadHeapProperties, D3D12_HEAP_FLAG_NONE,
		&resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&resultResource));
	assert(SUCCEEDED(hr));

	return resultResource;
}

D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(ID3D12DescriptorHeap* descriptorHeap, uint32_t descriptorSize, uint32_t index)
{
	D3D12_CPU_DESCRIPTOR_HANDLE handleCPU = descriptorHeap->GetCPUDescriptorHandleForHeapStart();
	handleCPU.ptr += (descriptorSize * index);
	return handleCPU;
}

D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(ID3D12DescriptorHeap* descriptorHeap, uint32_t descriptorSize, uint32_t index)
{
	D3D12_GPU_DESCRIPTOR_HANDLE handleGPU = descriptorHeap->GetGPUDescriptorHandleForHeapStart();
	handleGPU.ptr += (descriptorSize * index);
	return handleGPU;
}

ID3D12Resource* CreateTextureResource(ID3D12Device* device, const DirectX::TexMetadata& metadata)
{
	// metadataを基にReasourceの設定
	D3D12_RESOURCE_DESC resourceDesc{};
	resourceDesc.Width = UINT(metadata.width);	// Textureの幅
	resourceDesc.Height = UINT(metadata.height);	// Textureの高さ
	resourceDesc.MipLevels = UINT16(metadata.mipLevels);	// mipmapの数
	resourceDesc.DepthOrArraySize = UINT16(metadata.arraySize);	// 奥行き or 配列Textureの配列数
	resourceDesc.Format = metadata.format;	// TexutureのFormat
	resourceDesc.SampleDesc.Count = 1;	// サンプリングカウント。1固定
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION(metadata.dimension);	// Textureの次元数。普段使っているのは2次元

	// 利用するHeapの設定。非常に特殊な運用。02_04exで一般的なケース版がある
	D3D12_HEAP_PROPERTIES heapProperties{};
	heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;	// 細かい設定を行う
	//heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;	// WriteBackポリシーでVPUアクセス可能
	//heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;	// プロセッサ

	// Resourceの生成
	ID3D12Resource* resource = nullptr;
	HRESULT hr;
	hr = device->CreateCommittedResource(
		&heapProperties,	// Heapの設定
		D3D12_HEAP_FLAG_NONE,	// Heapの特殊な設定。特になし
		&resourceDesc,	// Resourceの設定
		D3D12_RESOURCE_STATE_COPY_DEST,	// 初回のResourceState。Textureは基本読むだけ
		nullptr,	// Clear最適値。使わないのでnullptr
		IID_PPV_ARGS(&resource));
	assert(SUCCEEDED(hr));
	return resource;
}

[[nodiscard]]
ID3D12Resource* UploadTextureData(ID3D12Resource* texture, const DirectX::ScratchImage& mipImages,
	ID3D12Device* device, ID3D12GraphicsCommandList* commandList)
{
	std::vector<D3D12_SUBRESOURCE_DATA> subresources;
	DirectX::PrepareUpload(device, mipImages.GetImages(), mipImages.GetImageCount(), mipImages.GetMetadata(), subresources);
	uint64_t intermediateSize = GetRequiredIntermediateSize(texture, 0, UINT(subresources.size()));
	ID3D12Resource* intermediateResource = CreateBufferResource(device, intermediateSize);
	UpdateSubresources(commandList, texture, intermediateResource, 0, 0, UINT(subresources.size()), subresources.data());

	// Textureへの転送後は利用できるよう、D3D12_RESOURCE_STATE_COPY_DESTからD3D12_RESOURCE_STATE_GENERIC_READへResourceStateを変更する
	D3D12_RESOURCE_BARRIER barrier{};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = texture;
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_GENERIC_READ;
	commandList->ResourceBarrier(1, &barrier);

	return intermediateResource;
}

void Instancing::Create()
{
	dxCommon_ = DirectXCommon::GetInstance();
	device_ = DirectXCommon::GetInstance()->GetDevice();

	// インスタンシング用
	D3D12_SHADER_RESOURCE_VIEW_DESC instancingSrvDesc{};
	instancingSrvDesc.Format = DXGI_FORMAT_UNKNOWN;
	instancingSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	instancingSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	instancingSrvDesc.Buffer.FirstElement = 0;
	instancingSrvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	instancingSrvDesc.Buffer.NumElements = kMaxCount_;
	instancingSrvDesc.Buffer.StructureByteStride = sizeof(TransformationMatrix);

	// マテリアル用のリソースを作る。今回はcolor1つ分のサイズを用意する
	materialResource_ = CreateBufferResource(device_, sizeof(MaterialInfo));
	// 書き込むためのアドレスを取得
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
	// 
	*materialData_ = MaterialInfo({ 1.0f, 1.0f, 1.0f, 1.0f }, { true });

	// Lighting用のリソース
	directionalLightResource_ = CreateBufferResource(device_, sizeof(DirectionalLight));
	directionalLightData_ = nullptr;
	directionalLightResource_->Map(0, nullptr, reinterpret_cast<void**>(&directionalLightData_));

	CreateData();
	CreateMesh();
	CreateTexture();

}

void Instancing::CreateData()
{
	instancingResource_ = CreateBufferResource(device_, sizeof(TransformationMatrix) * kMaxCount_);
	// データを書き込む
	instancingData_ = nullptr;
	// 書き込むためのアドレスを取得
	instancingResource_->Map(0, nullptr, reinterpret_cast<void**>(&instancingData_));
	// 単位行列を書き込んでおく
	for (uint32_t index = 0; index < kMaxCount_; ++index) {
		instancingData_[index].WVP = MatLib::MakeIdentity4x4();
		instancingData_[index].World = MatLib::MakeIdentity4x4();
	}

}

void Instancing::CreateMesh()
{
	// モデル読み込み
	modelData_ = LoadPlane("Resources", "plane.obj");
	// 頂点
	vertexResource_ = CreateBufferResource(device_, sizeof(VertexDataInst) * modelData_.vertices.size());
	// ビュー
	vbView.BufferLocation = vertexResource_->GetGPUVirtualAddress();
	vbView.SizeInBytes = UINT(sizeof(VertexDataInst) * modelData_.vertices.size());
	vbView.StrideInBytes = sizeof(VertexDataInst);	//

	// データを書き込む
	vertexData_ = nullptr;
	vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));
	std::memcpy(vertexData_, modelData_.vertices.data(), sizeof(VertexDataInst) * modelData_.vertices.size());

}

void Instancing::CreateTexture()
{
	DirectX::ScratchImage mipImage;
	Microsoft::WRL::ComPtr<ID3D12Resource> interMediateResource;
	mipImage = LoadTexture("Resources/uvChecker.png");

	const DirectX::TexMetadata& metadata = mipImage.GetMetadata();

	texResource_ = CreateTextureResource(device_, metadata);

	interMediateResource = UploadTextureData(texResource_.Get(), mipImage, device_, DirectXCommon::GetInstance()->GetCommandList());

	// SRVの設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc[1]{};


	srvDesc[0].Format = metadata.format;
	srvDesc[0].Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc[0].ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;	// 2Dテクスチャ
	srvDesc[0].Texture2D.MipLevels = UINT(metadata.mipLevels);

	// SRVを作成するDescriptorHeapの場所を決める
	textureSrvHandleCPU[0] = dxCommon_->GetSRV()->GetHeap()->GetCPUDescriptorHandleForHeapStart();
	textureSrvHandleGPU[0] = dxCommon_->GetSRV()->GetHeap()->GetGPUDescriptorHandleForHeapStart();
	// 先頭はImGuiが使っているのでその次を使う
	textureSrvHandleCPU[0].ptr += device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	textureSrvHandleGPU[0].ptr += device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	// SRVの生成
	device_->CreateShaderResourceView(texResource_.Get(), &srvDesc[0], textureSrvHandleCPU[0]);

	// インスタンシング用
	D3D12_SHADER_RESOURCE_VIEW_DESC instancingSrvDesc{};
	instancingSrvDesc.Format = DXGI_FORMAT_UNKNOWN;
	instancingSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	instancingSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	instancingSrvDesc.Buffer.FirstElement = 0;
	instancingSrvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	instancingSrvDesc.Buffer.NumElements = kMaxCount_;
	instancingSrvDesc.Buffer.StructureByteStride = sizeof(TransformationMatrix);

	const uint32_t descriptorSizeSRV = device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	ID3D12DescriptorHeap* heap = dxCommon_->GetSRV()->GetHeap();

	instancingSrvHandleCPU = GetCPUDescriptorHandle(heap, descriptorSizeSRV, 3);
	instancingSrvHandleGPU = GetGPUDescriptorHandle(heap, descriptorSizeSRV, 3);

	device_->CreateShaderResourceView(instancingResource_.Get(), &instancingSrvDesc, instancingSrvHandleCPU);

}

void Instancing::Initialize()
{
	instancingCount_ = 10;

	for (uint32_t i = 0; i < kMaxCount_; ++i) {
		transforms[i].scale = { 1,1,1 };
		transforms[i].rotate = {};
		transforms[i].translate = { i * 0.1f,i * 0.1f,i * 0.1f };
	}
	cameraTransform_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,-10.0f} };
}

void Instancing::Update()
{

	ImGuiWidget();

	UpdateMatrix();


}

void Instancing::UpdateMatrix()
{
	Matrix4x4 cameraMatrix = MatLib::MakeAffineMatrix(cameraTransform_.scale, cameraTransform_.rotate, cameraTransform_.translate);
	Matrix4x4 viewMatrix = MatLib::MakeInverse(cameraMatrix);
	float kClientWidth = WindowAPI::kClientWidth;
	float kClientHeight = WindowAPI::kClientHeight;
	Matrix4x4 projectionMatrix = MatLib::MakePerspectiveFovMatrix(0.45f, float(kClientWidth) / float(kClientHeight), 0.1f, 100.0f);

	for (uint32_t i = 0; i < kMaxCount_; ++i) {
		Matrix4x4 worldMatrix = MatLib::MakeAffineMatrix(transforms[i].scale, transforms[i].rotate, transforms[i].translate);
		Matrix4x4 worldViewProjectionMatrix = MatLib::Multiply(worldMatrix, MatLib::Multiply(viewMatrix, projectionMatrix));

		instancingData_[i].WVP = worldViewProjectionMatrix;
		instancingData_[i].World = worldMatrix;

		//tag[i] = name + std::to_string(i);

	}
}

void Instancing::ImGuiWidget()
{
	ImGui::Begin("Instancing");
	
	ImGui::DragFloat3("Camera", &cameraTransform_.translate.x);
	
	for (int i = 0; i < (int)kMaxCount_; ++i) {
		std::string name = "Pos" + std::to_string(i);
		ImGui::DragFloat3(name.c_str(), &transforms[i].translate.x);
	}

	ImGui::End();
}

void Instancing::Draw()
{
	sCommandList_->SetPipelineState(PipelineManager::sParticlePipelineStates_.Get());

	ID3D12DescriptorHeap* descriptorHeaps[] = { dxCommon_->GetSRV()->GetHeap() };
	sCommandList_->SetDescriptorHeaps(1, descriptorHeaps);

	// マテリアル情報
	sCommandList_->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
	// テーブル
	sCommandList_->SetGraphicsRootDescriptorTable(1, instancingSrvHandleGPU);
	// テクスチャ
	sCommandList_->SetGraphicsRootDescriptorTable(2, textureSrvHandleGPU[0]);
	// ライティング
	sCommandList_->SetGraphicsRootConstantBufferView(3, directionalLightResource_->GetGPUVirtualAddress());

	sCommandList_->IASetVertexBuffers(0, 1, &vbView);
	// インデックスバッファをセット
	//sCommandList_->IASetIndexBuffer(&vbView);

	sCommandList_->DrawInstanced(UINT(modelData_.vertices.size()), kMaxCount_, 0, 0);

}

void Instancing::PreDraw(ID3D12GraphicsCommandList* commandList)
{
	assert(sCommandList_ == nullptr);

	// 設定
	sCommandList_ = commandList;

	// シグネチャ

	commandList->SetGraphicsRootSignature(PipelineManager::sParticleRootSignature_.Get());

	// 形状設定
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Instancing::PostDraw()
{
	// 解除
	sCommandList_ = nullptr;
}

ModelData Instancing::LoadPlane(const std::string& directory, const std::string& fileName)
{
	ModelData modelData;
	std::vector<Vector4> positions;
	std::vector<Vector3> normals;
	std::vector<Vector2> texcoords;
	std::string line;

	std::ifstream file(directory + "/" + fileName);
	assert(file.is_open());

	// 読み込み
	while (std::getline(file, line))
	{
		std::string identifier;
		std::istringstream s(line);

		s >> identifier;
#pragma region ファイル読み込み
		if (identifier == "v") {
			Vector4 pos;
			s >> pos.x >> pos.y >> pos.z;
			pos.w = 1.0f;
			positions.push_back(pos);
		}
		else if (identifier == "vt") {
			Vector2 texcoord;
			s >> texcoord.x >> texcoord.y;
			// V方向反転
			texcoord.y = 1.0f - texcoord.y;
			texcoords.push_back(texcoord);
		}
		else if (identifier == "vn") {
			Vector3 normal;
			s >> normal.x >> normal.y >> normal.z;
			normals.push_back(normal);
		}
		else if (identifier == "f") {
			// 三角形限定
			VertexDataInst triangle[3];

			for (int32_t faceVertex = 0; faceVertex < 3; ++faceVertex) {
				std::string vertexDefinition;
				s >> vertexDefinition;
				// 
				std::istringstream v(vertexDefinition);
				uint32_t elementIndices[3];
				for (int32_t element = 0; element < 3; ++element) {
					std::string index;
					std::getline(v, index, '/');
					elementIndices[element] = std::stoi(index);
				}

				// 
				Vector4 position = positions[elementIndices[0] - 1];
				Vector2 texcoord = texcoords[elementIndices[1] - 1];
				Vector3 normal = normals[elementIndices[2] - 1];
				//position.x *= -1.0f;
				normal.x *= -1.0f;
				//VertexData vertex = { position,texcoord,normal };
				//modelData.vertices.push_back(vertex);
				triangle[faceVertex] = { position,texcoord,normal };
			}
			modelData.vertices.push_back(triangle[2]);
			modelData.vertices.push_back(triangle[1]);
			modelData.vertices.push_back(triangle[0]);
		}
		// マテリアル読み込み
		else if (identifier == "mtllib") {
			std::string materialFilename;

			s >> materialFilename;
			modelData.material = LoadMaterial(directory, materialFilename);
		}
#pragma endregion
	}

	return modelData;
}

MaterialData Instancing::LoadMaterial(const std::string& directory, const std::string& fileName)
{
	MaterialData materialData;
	std::string line;
	std::ifstream file(directory + "/" + fileName);
	assert(file.is_open());

	while (std::getline(file, line))
	{
		std::string identifier;
		std::istringstream s(line);
		s >> identifier;

		if (identifier == "map_Kd") {
			std::string textureFilename;
			s >> textureFilename;
			// ファイルパス
			materialData.textureFilePath = directory + "/" + textureFilename;
		}
	}


	return materialData;
}

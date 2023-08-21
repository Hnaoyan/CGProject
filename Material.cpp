#include "Material.h"
#include "TextureManager.h"
#include "DirectXCommon.h"
#include "D3D12Lib/D3D12Lib.h"

Material* Material::Create()
{
	Material* instance = new Material;
	instance->Initialize();
	return instance;
}

void Material::Initialize()
{
	// 定数バッファの生成
	CreateConstantBuffer();
}

void Material::CreateConstantBuffer()
{
	HRESULT result;
	// ヒーププロパティ
	D3D12_HEAP_PROPERTIES heapProps = D3D12Lib::SetHeapProperties(D3D12_HEAP_TYPE_UPLOAD);
	// リソース設定
	D3D12_RESOURCE_DESC resourceDesc = D3D12Lib::SetResourceDesc((sizeof(ConstBufferData) + 0xff) & ~0xff);

	// 定数バッファの生成
	result = DirectXCommon::GetInstance()->GetDevice()->CreateCommittedResource(
		&heapProps, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&constBuff_));
	assert(SUCCEEDED(result));

	// 定数バッファとのデータリンク
	result = constBuff_->Map(0, nullptr, (void**)&constMap_);
	assert(SUCCEEDED(result));

}

void Material::LoadTexture(const std::string& directoryPath)
{
	// テクスチャがなかった場合
	if (textureFilename_.size() == 0) {
		textureFilename_ = "white1x1.png";
	}

	//HRESULT result = S_FALSE;
	// WICテクスチャのロード
	DirectX::TexMetadata metadata{};
	DirectX::ScratchImage scratchImg{};
	// ファイルパス
	std::string filepath = directoryPath + textureFilename_;

	// テクスチャ読み込み
	textureHandle_ = TextureManager::Load(filepath);

}

void Material::Update()
{
	// 定数バッファのデータ転送
	constMap_->ambient = ambient_;
	constMap_->diffuse = diffuse_;
	constMap_->specular = specular_;
	constMap_->alpha = alpha_;
}

void Material::SetGraphicsCommand(ID3D12GraphicsCommandList* commandList, UINT rootParametreIndexMaterial, UINT rootParameterIndexTexture)
{
	// SRVをセット
	TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(commandList, rootParameterIndexTexture, textureHandle_);

	// マテリアルの定数バッファをセット
	commandList->SetGraphicsRootConstantBufferView(rootParametreIndexMaterial, constBuff_->GetGPUVirtualAddress());

}

void Material::SetGraphicsCommand(ID3D12GraphicsCommandList* commandList, UINT rootParametreIndexMaterial, UINT rootParameterIndexTexture, uint32_t textureHandle)
{
	// SRVをセット
	TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(commandList, rootParameterIndexTexture, textureHandle);

	// マテリアルの定数バッファをセット
	commandList->SetGraphicsRootConstantBufferView(rootParametreIndexMaterial, constBuff_->GetGPUVirtualAddress());

}

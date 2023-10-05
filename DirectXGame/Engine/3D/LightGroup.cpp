#include "LightGroup.h"
#include <cassert>
#include "DirectXCommon.h"
#include "D3D12Lib.h"

LightGroup* LightGroup::Create()
{
	// 3Dオブジェクトのインスタンスを生成
	LightGroup* instance = new LightGroup();

	// 初期化
	instance->Initialize();

	return instance;
}

void LightGroup::Initialize()
{
	DefaultLightSetting();

	// ヒーププロパティ
	D3D12_HEAP_PROPERTIES heapProps = D3D12Lib::SetHeapProperties(D3D12_HEAP_TYPE_UPLOAD);
	// リソース設定
	D3D12_RESOURCE_DESC resourceDesc =
		D3D12Lib::SetResourceDesc((sizeof(ConstBufferData) + 0xff) & ~0xff);

	HRESULT result;
	// 定数バッファの生成
	result = DirectXCommon::GetInstance()->GetDevice()->CreateCommittedResource(
		&heapProps,
		D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&constBuff_));
	assert(SUCCEEDED(result));
	// 定数バッファとのデータリンク
	result = constBuff_->Map(0, nullptr, (void**)&constMap_);
	assert(SUCCEEDED(result));

	// 定数バッファへデータ転送
	TransferConstBuffer();

}

void LightGroup::Update()
{
	// 値の更新があった時だけ定数バッファに転送する
	if (dirty_) {
		TransferConstBuffer();
		dirty_ = false;
	}
}

void LightGroup::Draw(ID3D12GraphicsCommandList* cmdList, UINT rootParameterIndex)
{
	// 定数バッファビューをセット
	cmdList->SetGraphicsRootConstantBufferView(
		rootParameterIndex, constBuff_->GetGPUVirtualAddress());

}

void LightGroup::TransferConstBuffer()
{
	// 環境光
	constMap_->ambientColor = ambientColor_;
	// 平行光源
	for (int i = 0; i < kDirLightNum; i++) {
		if (dirLights_[i].IsActive()) {
			constMap_->dirLights[i].active = 1;
			constMap_->dirLights[i].lightVector = Vector4(-dirLights_[i].GetLightDir().x, -dirLights_[i].GetLightDir().y,
				-dirLights_[i].GetLightDir().z, -dirLights_[i].GetLightDir().w);
			constMap_->dirLights[i].lightColor = dirLights_[i].GetLightColor();
		}
		// ライトが無効
		else {
			constMap_->dirLights[i].active = 0;
		}
	}

}

void LightGroup::DefaultLightSetting()
{
	dirLights_[0].SetActive(true);
	dirLights_[0].SetLightColor({ 1.0f,1.0f,1.0f });
	dirLights_[0].SetLightDir({ 0.0f,-1.0f,0.0f,0 });

	dirLights_[1].SetActive(true);
	dirLights_[1].SetLightColor({ 1.0f,1.0f,1.0f });
	dirLights_[1].SetLightDir({ +0.5f,+1.0f,+0.2f,0 });

	dirLights_[2].SetActive(true);
	dirLights_[2].SetLightColor({ 1.0f,1.0f,1.0f });
	dirLights_[2].SetLightDir({ -0.5f,+0.1f,-0.2f,0 });
}

void LightGroup::SetAmbientColor(const Vector3& color)
{
	ambientColor_ = color;
	dirty_ = true;
}

void LightGroup::SetDirLightActive(int index, bool active)
{
	assert(0 <= index && index < kDirLightNum);

	dirLights_[index].SetActive(active);
}

void LightGroup::SetDirLightDir(int index, const Vector4& lightdir)
{
	assert(0 <= index && index < kDirLightNum);

	dirLights_[index].SetLightDir(lightdir);
	dirty_ = true;
}

void LightGroup::SetDirLightColor(int index, const Vector3& lightcolor)
{
	assert(0 <= index && index < kDirLightNum);

	dirLights_[index].SetLightColor(lightcolor);
	dirty_ = true;
}

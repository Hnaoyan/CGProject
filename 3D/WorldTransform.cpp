#include "WorldTransform.h"
#include "D3D12Lib.h"
#include "DirectXCommon.h"
#include <cassert>
#include "Matrix.h"
#include "MatrixMath.h"

void WorldTransform::Initialize()
{
	this->CreateConstBuffer();
	this->Map();
	this->UpdateMatrix();
}

void WorldTransform::CreateConstBuffer()
{
	HRESULT result;

	D3D12_HEAP_PROPERTIES heapProps = D3D12Lib::SetHeapProperties(D3D12_HEAP_TYPE_UPLOAD);

	D3D12_RESOURCE_DESC resourceDesc = D3D12Lib::SetResourceDesc((sizeof(ConstBufferDataWorldTransform) + 0xff) & ~0xff);

	result = DirectXCommon::GetInstance()->GetDevice()->CreateCommittedResource(
		&heapProps,
		D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&constBuff_));
	assert(SUCCEEDED(result));

}

void WorldTransform::Map()
{
	HRESULT result = constBuff_->Map(0, nullptr, (void**)&constMap);
	assert(SUCCEEDED(result));
}

void WorldTransform::TransferMatrix()
{
	constMap->matWorld = matWorld_;
}

void WorldTransform::UpdateMatrix() {
	this->matWorld_ =
		MatrixMath::MakeAffineMatrix(this->scale_, this->rotation_, this->translation_);

	// 定数バッファに転送
	TransferMatrix();
}
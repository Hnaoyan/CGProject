#include "WorldTransform.h"
#include "D3D12Lib/D3D12Lib.h"
#include "DirectXCommon.h"
#include <cassert>
#include "Matrix.h"

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

void WorldTransform::UpdateMatrix()
{
	Matrix4x4 matScale, matRotate, matTranslate;

	matScale = MakeScaleMatrix(scale_);
	matRotate = MakeIdentity4x4();
	matRotate = Multiply(MakeRotateXMatrix(rotation_.x),
		Multiply(MakeRotateZMatrix(rotation_.z), MakeRotateYMatrix(rotation_.y)));

	matTranslate = MakeTranslateMatrix(translation_);

	matWorld_ = MakeIdentity4x4();
	matWorld_ = Multiply(matWorld_, matScale);
	matWorld_ = Multiply(matWorld_, matRotate);
	matWorld_ = Multiply(matWorld_, matTranslate);

	constMap->matWorld = matWorld_;
}

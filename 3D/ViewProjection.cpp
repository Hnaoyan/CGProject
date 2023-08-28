#include "ViewProjection.h"
#include "D3D12Lib.h"
#include "DirectXCommon.h"
#include <cassert>
#include "Matrix.h"
#include "MathCalc.h"
#include "Vector3Math.h"

void ViewProjection::Initialize()
{
	this->CreateConstBuffer();
	this->Map();
	this->UpdateMatrix();
}

void ViewProjection::CreateConstBuffer()
{
	HRESULT result;

	D3D12_HEAP_PROPERTIES heapProps = D3D12Lib::SetHeapProperties(D3D12_HEAP_TYPE_UPLOAD);

	D3D12_RESOURCE_DESC resourceDesc = D3D12Lib::SetResourceDesc((sizeof(ConstBufferDataViewProjection) + 0xff) & ~0xff);

	result = DirectXCommon::GetInstance()->GetDevice()->CreateCommittedResource(
		&heapProps,
		D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&constBuff_));
	assert(SUCCEEDED(result));
}

void ViewProjection::Map()
{
	HRESULT result = constBuff_->Map(0, nullptr, (void**)&constMap);
	assert(SUCCEEDED(result));
}

void ViewProjection::UpdateMatrix()
{
	Matrix4x4 matCamera = MakeAffineMatrix(scale_, rotation_, translate_);
	matView = MakeInverse(matCamera);
	matProjection = MakePerspectiveFovMatrix(fovAngleY, aspectRatio, nearZ, farZ);
	TransferMatrix();
}

void ViewProjection::TransferMatrix()
{
	constMap->view = matView;
	constMap->projection = matProjection;

	Matrix4x4 matTranslate = MakeTranslateMatrix(translate_);

	constMap->cameraPos.x = matTranslate.m[3][0];
	constMap->cameraPos.y = matTranslate.m[3][1];
	constMap->cameraPos.z = matTranslate.m[3][2];

}

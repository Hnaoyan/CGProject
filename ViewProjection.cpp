#include "ViewProjection.h"
#include "D3D12Lib/D3D12Lib.h"
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
	Matrix4x4 view;
	
	Vector3 forward = MathCalc::Normalize(Vector3Math::Subtract(target, eye));
	Vector3 right = MathCalc::Normalize(MathCalc::Cross(up, forward));
	Vector3 actual_up = MathCalc::Cross(forward, right);

	view.m[0][0] = right.x;
	view.m[1][0] = right.y;
	view.m[2][0] = right.z;
	view.m[3][0] = 0.0f;
	
	view.m[0][1] = actual_up.x;
	view.m[1][1] = actual_up.y;
	view.m[2][1] = actual_up.z;
	view.m[3][1] = 0.0f;

	view.m[0][2] = -forward.x;
	view.m[1][2] = -forward.y;
	view.m[2][2] = -forward.z;
	view.m[3][2] = 0.0f;


	view.m[0][3] = 0.0f;
	view.m[1][3] = 0.0f;
	view.m[2][3] = 0.0f;
	view.m[3][3] = 1.0f;


	matView = view;

	matProjection = MakePerspectiveFovMatrix(fovAngleY, aspectRatio, nearZ, farZ);

	constMap->view = matView;
	constMap->projection = matProjection;
	constMap->cameraPos = eye;

}

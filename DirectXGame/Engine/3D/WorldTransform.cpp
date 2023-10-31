#include "WorldTransform.h"
#include "D3D12Lib.h"
#include "DirectXCommon.h"
#include <cassert>
#include "MathCalc.h"
#include <numbers>

void WorldTransform::Initialize()
{
	// バッファーの作成
	this->CreateConstBuffer();
	// マッピング
	this->Map();
	// 行列の送信
	this->TransferMatrix();
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
	HRESULT result;
	result = constBuff_->Map(0, nullptr, (void**)&constMap);
	assert(SUCCEEDED(result));
}

void WorldTransform::TransferMatrix()
{
	constMap->matWorld = matWorld_;
}

void WorldTransform::BillBoardSetting(ViewProjection* view, bool billBoardFlag)
{
	// カメラの設定
	view_ = view;
	// ビルボードの切り替え
	isBillBoard_ = billBoardFlag;
}



void WorldTransform::UpdateMatrix() {
	// ビルボードありの場合
	if (isBillBoard_) {
		Matrix4x4 billBoardMat = MatLib::MakeBillBoard(
			translation_, view_->translate_, Vector3(0.0f, 1.0f, 0.0f));

		Matrix4x4 worldMat = MatLib::MakeAffineMatrix(
			scale_, rotation_, translation_);
		matWorld_ = MatLib::Multiply(billBoardMat, worldMat);
	}
	// ビルボードなしの場合
	else {
		this->matWorld_ =
			MatLib::MakeAffineMatrix(this->scale_, this->rotation_, this->translation_);
	}

	//親があれば親のワールド行列を掛ける
	if (parent_) {
		matWorld_ = MatLib::Multiply(matWorld_, parent_->matWorld_);
	}

	// 定数バッファに転送
	TransferMatrix();
}
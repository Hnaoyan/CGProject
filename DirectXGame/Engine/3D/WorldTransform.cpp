#include "WorldTransform.h"
#include "D3D12Lib.h"
#include "DirectXCommon.h"
#include <cassert>
#include "MathCalc.h"
#include <numbers>
#include "imgui.h"

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

void WorldTransform::BillBoardSetting(const ViewProjection* viewProjection, bool billBoardFlag)
{
	// カメラの設定
	viewProjection_ = viewProjection;
	// ビルボードの切り替え
	isBillBoard_ = billBoardFlag;
}

void WorldTransform::ImGuiWidget(const char* tag)
{

	ImGui::Begin(tag);
	float posValue = 500.0f;
	ImGui::DragFloat3("position", &this->translation_.x, 0.01f, -posValue, posValue);
	ImGui::DragFloat3("rotate", &this->rotation_.x, 0.01f, -5.0f, 5.0f);
	ImGui::End();

}



void WorldTransform::UpdateMatrix() {
	// ビルボードありの場合
	if (isBillBoard_) {
		Matrix4x4 billBoardMat = MatLib::MakeBillBoard(
			translation_, viewProjection_->translate_, Vector3(0.0f, 1.0f, 0.0f));

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
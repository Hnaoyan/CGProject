#include "Area.h"

void Area::Initialize(Model* model, Model* yellowLineModel)
{

	// ワールドトランスフォーム
	worldTransform_.Initialize();

	// モデル
	model_ = model;

	// ワールドトランスフォーム
	yellowLineWorldTransform_.Initialize();

	// モデル
	yellowLineModel_ = yellowLineModel;

	Setting();

}

void Area::Update()
{

	worldTransform_.UpdateMatrix();
	yellowLineWorldTransform_.UpdateMatrix();

}

void Area::Draw(const ViewProjection& viewProjection)
{

	model_->Draw(worldTransform_, viewProjection);
	yellowLineModel_->Draw(yellowLineWorldTransform_, viewProjection);

}

void Area::Setting()
{

	worldTransform_.translation_ = { 0.0f,0.0f,0.0f };
	worldTransform_.rotation_ = { 0.0f,0.0f,0.0f };
	worldTransform_.scale_ = { 1.0f,1.0f,1.0f };
	worldTransform_.UpdateMatrix();

	yellowLineWorldTransform_.translation_ = { kPositionMax_.x / 2.0f ,kYellowLine_ - 1.0f,-1.0f };
	yellowLineWorldTransform_.rotation_ = { 0.0f,0.0f,0.0f };
	yellowLineWorldTransform_.scale_ = { 1.0f,1.0f,1.0f };
	yellowLineWorldTransform_.UpdateMatrix();

}

#include "SampleMissile.h"

void SampleMissile::Initialize(Model* model)
{

	model_ = model;
	// ワールドトランスフォーム
	worldTransform_.Initialize();
	worldTransform_.UpdateMatrix();

}

void SampleMissile::Update()
{

	worldTransform_.UpdateMatrix();
}

void SampleMissile::Draw(const ViewProjection& viewProjection)
{

	model_->Draw(worldTransform_, viewProjection);

}

#include "Goal.h"

void Goal::Initialize(Model* model)
{
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.translation_ = { 0.0f,0.0f,50.0f };
	radius_ = 1.0f;
	SetterRad(radius_);
	SetPosition(worldTransform_.translation_);
	SetCollisionAttribute(kCollisionAttributeGoal);

}

void Goal::Update()
{
	worldTransform_.UpdateMatrix();
}

void Goal::Draw(const ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection);
}

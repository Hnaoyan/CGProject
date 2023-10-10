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
	SetWorldAddress(&worldTransform_);
	// バインド
	std::function<void(uint32_t, WorldTransform*)> f = std::function<void(uint32_t, WorldTransform*)>(std::bind(&Goal::OnCollision, this, std::placeholders::_1, std::placeholders::_2));
	SetFunction(f);

}

void Goal::Update()
{
	worldTransform_.UpdateMatrix();
}

void Goal::Draw(const ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection);
}

void Goal::OnCollision(uint32_t tag, WorldTransform* world)
{
	tag;
	world;
}

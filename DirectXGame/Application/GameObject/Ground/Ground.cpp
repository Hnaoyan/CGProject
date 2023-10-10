#include "Ground.h"
#include <cassert>

void Ground::Initialize(Model* model) 
{ 
	model_ = model;
	worldTransform_.Initialize();
	collider_.SetWorldAddress(&worldTransform_);
	std::function<void(uint32_t, WorldTransform*)> f = std::function<void(uint32_t, WorldTransform*)>(std::bind(&Ground::OnCollision, this, std::placeholders::_1, std::placeholders::_2));
	collider_.SetFunction(f);
}

void Ground::Update() 
{
	collider_.SetPosition(worldTransform_.translation_);
	worldTransform_.UpdateMatrix();
}

void Ground::Draw(const ViewProjection& viewProjection) 
{
	model_->Draw(worldTransform_, viewProjection);
}

void Ground::SetPosition(const Vector3& pos)
{
	worldTransform_.translation_ = pos;
}

void Ground::OnCollision(uint32_t tag, WorldTransform* world)
{
	tag;
	world;
}


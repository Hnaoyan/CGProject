#include "Ground.h"
#include <cassert>

void Ground::Initialize(Model* model) 
{ 
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.scale_ = { 2.0f,0,2.0f };
	collider_.SetWorldAddress(&worldTransform_);
	std::function<void(uint32_t, WorldTransform*)> f = std::function<void(uint32_t, WorldTransform*)>(std::bind(&Ground::OnCollision, this, std::placeholders::_1, std::placeholders::_2));
	collider_.SetFunction(f);
}

void Ground::Update() 
{
	if (collider_.GetAttribute() == kCollisionAttributeMoveGround) {
		worldTransform_.translation_.x += 0.01f;
	}
	//collider_.SetPosition(worldTransform_.translation_);
	worldTransform_.UpdateMatrix();
	Vector3 position = { worldTransform_.matWorld_.m[3][0],worldTransform_.matWorld_.m[3][1],worldTransform_.matWorld_.m[3][2] };
	collider_.SetPosition(position);

	//worldTransform_.parent_ = nullptr;
}

void Ground::Draw(const ViewProjection& viewProjection) 
{
	model_->Draw(worldTransform_, viewProjection);
}

void Ground::SetPosition(const Vector3& pos)
{
	worldTransform_.translation_ = pos;
}

void Ground::SetScale(const Vector3& scale)
{
	worldTransform_.scale_ = scale;
}

void Ground::OnCollision(uint32_t tag, WorldTransform* world)
{
	if (collider_.GetAttribute() == kCollisionAttributeMoveGround &&
		tag == kCollisionAttributePlayer) {
		//worldTransform_.parent_ = world;
		world;
	}
}


#include "Ground.h"
#include <cassert>

void Ground::Initialize(Model* model) 
{ 
	assert(model);
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.translation_ = {0, 0, 0};

	aabb_.min = { worldTransform_.translation_.x - radius_,worldTransform_.translation_.y - 0.01f,worldTransform_.translation_.z - radius_ };
	aabb_.max = { worldTransform_.translation_.x + radius_,worldTransform_.translation_.y + 0.01f,worldTransform_.translation_.z + radius_ };

}

void Ground::Update() 
{
	aabb_.min = { worldTransform_.translation_.x - radius_,worldTransform_.translation_.y - 0.01f,worldTransform_.translation_.z - radius_ };
	aabb_.max = { worldTransform_.translation_.x + radius_,worldTransform_.translation_.y + 0.01f,worldTransform_.translation_.z + radius_ };
	worldTransform_.UpdateMatrix(); 
}

void Ground::Draw(const ViewProjection& viewProjection) 
{
	model_->Draw(worldTransform_, viewProjection);
}

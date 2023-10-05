#include "Ground.h"
#include <cassert>

void Ground::Initialize(Model* model) 
{ 
	assert(model);
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.translation_ = {0, 0, 0};
}

void Ground::Update() { worldTransform_.UpdateMatrix(); }

void Ground::Draw(const ViewProjection& viewProjection) 
{
	model_->Draw(worldTransform_, viewProjection);
}

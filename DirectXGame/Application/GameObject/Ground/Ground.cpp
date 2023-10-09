#include "Ground.h"
#include <cassert>

void Ground::Initialize(Model* model) 
{ 
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.translation_ = {0, 0, 0};
	worldTransform_.scale_ = { 1.0f,1.0f,1.0f };
	SetPosition(worldTransform_.translation_);
}

void Ground::Update() { worldTransform_.UpdateMatrix(); }

void Ground::Draw(const ViewProjection& viewProjection) 
{
	model_->Draw(worldTransform_, viewProjection);
}

void Ground::SetPosition(const Vector3& pos)
{
	worldTransform_.translation_ = pos;
}

void MoveGround::Initialize(Model* model)
{
	Ground::Initialize(model);
}

void MoveGround::Update()
{
	//worldTransform_.translation_.x
	Ground::Update();
}

void MoveGround::Draw(const ViewProjection& viewProjection)
{
	Ground::Draw(viewProjection);
}

void NormalGround::Initialize(Model* model)
{
	Ground::Initialize(model);
}

void NormalGround::Update()
{
	Ground::Update();
}

void NormalGround::Draw(const ViewProjection& viewProjection)
{
	Ground::Draw(viewProjection);
}

#include "Goal.h"
#include "TextureManager.h"

void Goal::Initialize(Model* model)
{
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.translation_ = { 0.0f,1.0f,70.0f };
	radius_ = 1.0f;
	SetterRad(Vector3(radius_, radius_, radius_));
	SetPosition(worldTransform_.translation_);
	SetCollisionAttribute(kCollisionAttributeGoal);
	SetWorldAddress(&worldTransform_);
	// バインド
	std::function<void(uint32_t, WorldTransform*)> f = std::function<void(uint32_t, WorldTransform*)>(std::bind(&Goal::OnCollision, this, std::placeholders::_1, std::placeholders::_2));
	SetFunction(f);
	textureHandle_ = TextureManager::Load("white1x1.png");
}

void Goal::Update()
{
	worldTransform_.UpdateMatrix();
}

void Goal::Draw(const ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection,textureHandle_);
}

void Goal::OnCollision(uint32_t tag, WorldTransform* targetWorldTransform)
{
	tag;
	targetWorldTransform;
}

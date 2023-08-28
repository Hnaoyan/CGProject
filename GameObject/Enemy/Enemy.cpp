#include "Enemy.h"
#include "ImGuiManager.h"
#include "CollisionConfig.h"

void Enemy::Initialize(Model* model) 
{
	BaseCharacter::Initialize(model); 
	worldTransform_.translation_ = {0, 1.0f, 30.0f};
	worldTransform_.rotation_.y = 3.1f;
	this->SetCollisionAttribute(kCollisionAttributeEnemy);
	this->SetCollisionMask(kCollisionAttributePlayer);
}

void Enemy::Update() 
{ 
	//ImGui::Begin("enemy");
	//ImGui::DragFloat3("translation", &worldTransform_.translation_.x, 0.01f, -20.0f, 20.0f);
	//ImGui::DragFloat3("rotate", &worldTransform_.rotation_.x, 0.01f, -5.0f, 5.0f);
	//ImGui::End();

	BaseCharacter::Update();
}

void Enemy::Draw(const ViewProjection& viewProjection) 
{ 
	BaseCharacter::Draw(viewProjection);
}

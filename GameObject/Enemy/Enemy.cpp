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

	info_.hp = 10;
	info_.invisibleCount = 0;
	info_.isHit = false;
	info_.isDead_ = false;
}

void Enemy::Update() 
{ 
	ImGui::Begin("enemy");
	ImGui::DragFloat3("translation", &worldTransform_.translation_.x, 0.01f, -20.0f, 20.0f);
	ImGui::DragFloat3("rotate", &worldTransform_.rotation_.x, 0.01f, -5.0f, 5.0f);
	ImGui::End();
	if (info_.hp <= 0) {
		info_.isDead_ = true;
	}
	if (info_.isDead_) {
		delete this;
	}
	BaseCharacter::Update();
}

void Enemy::Draw(const ViewProjection& viewProjection) 
{ 
	BaseCharacter::Draw(viewProjection);
}

void Enemy::OnCollision()
{
	if (info_.isHit) {
		//info_.hp -= 1;
		info_.isDead_ = true;
	}
}

#include "Enemy.h"
#include "ImGuiManager.h"
#include "CollisionConfig.h"

void Enemy::Initialize(Model* model) 
{
	BaseCharacter::Initialize(model); 
	worldTransform_.translation_ = {0, 3.0f, 30.0f};
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
	BaseCharacter::Update();
}

void Enemy::Draw(const ViewProjection& viewProjection) 
{ 
	//if (!info_.isDead_) {
		BaseCharacter::Draw(viewProjection);
	//}
}

void Enemy::OnCollision()
{
	info_.isDead_ = true;
}

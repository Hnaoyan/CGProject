#include "Weapon.h"
#include "imgui.h"
#include "Enemy.h"

void Weapon::Initialize()
{
	worldTransform_.Initialize();
	isAttack_ = false;
	float radius = 2.0f;

	SetterRad(Vector3{ radius,radius,radius });
	SetWorldAddress(&worldTransform_);
	SetCollisionAttribute(kCollisionAttributeWeapon);
	SetCollisionMask(0xffffffff - kCollisionAttributeWeapon);
	// 関数バインド
	std::function<void(uint32_t, WorldTransform*)> f = std::function<void(uint32_t, WorldTransform*)>(std::bind(&Weapon::OnCollision, this, std::placeholders::_1, std::placeholders::_2));
	SetFunction(f);

}

void Weapon::Update()
{
	worldTransform_.UpdateMatrix();
	Vector3 pos = { worldTransform_.matWorld_.m[3][0],worldTransform_.matWorld_.m[3][1],worldTransform_.matWorld_.m[3][2] };
	SetPosition(pos);
	pos = GetPosition();
}

void Weapon::OnCollision(uint32_t tag, WorldTransform* worldTransfrom)
{
	tag, worldTransfrom;
}

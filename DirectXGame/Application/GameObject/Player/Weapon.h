#pragma once
#include "Collider.h"
#include "WorldTransform.h"
#include "ContactRecord.h"

class Weapon : public Collider
{
public:
	void Initialize();
	void Update();

	void SetWorldPosition(const Vector3& position) { worldTransform_.translation_ = position; }
	void SetScale(const Vector3& scale) { worldTransform_.scale_ = scale; }

	WorldTransform GetWorldTransform() { return worldTransform_; }

	void SetIsAttack(bool isAttack) { isAttack_ = isAttack; }
	bool GetIsAttack() { return isAttack_; }

private:
	void OnCollision(uint32_t tag, WorldTransform* worldTransfrom);

	WorldTransform worldTransform_;

	ContactRecord contactRecord_;

	bool isAttack_ = false;
};


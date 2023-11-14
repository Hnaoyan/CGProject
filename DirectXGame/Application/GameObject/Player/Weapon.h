#pragma once
#include "Collider.h"
#include "WorldTransform.h"

class Weapon : public Collider
{
public:
	void Initialize();
	void Update();

	void SetWorldPosition(const Vector3& position) { worldTransform_.translation_ = position; }

	WorldTransform GetWorldTransform() { return worldTransform_; }

private:
	void OnCollision(uint32_t tag, WorldTransform* worldTransfrom);

	WorldTransform worldTransform_;

};


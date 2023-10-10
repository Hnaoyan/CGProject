#include "Collider.h"

void Collider::SetPosition(Vector3& pos)
{
	position_ = pos;
}

void Collider::OnCollision(uint32_t tag, WorldTransform* world)
{
	function_(tag, world);
}

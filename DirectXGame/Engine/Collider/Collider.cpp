#include "Collider.h"

void Collider::SetPosition(Vector3& pos)
{
	position_ = pos;
}

void Collider::OnCollision(uint32_t tag, Vector3* position)
{

	function_(tag, position);

}

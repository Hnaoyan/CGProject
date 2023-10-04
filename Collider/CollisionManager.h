#pragma once
#include "Collider.h"
#include "StructManager.h"

#include <vector>

class CollisionManager
{
private:
	// リスト
	std::vector<Collider*> colliders_;

public:

	void CheckAllCollisions();

};


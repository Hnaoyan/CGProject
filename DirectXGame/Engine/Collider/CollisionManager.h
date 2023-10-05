#pragma once
#include "Collider.h"
#include "StructManager.h"

#include <list>

class CollisionManager
{
private:
	// リスト
	std::list<Collider*> colliders_;

public:
	/// <summary>
	/// すべてのオブジェクトの衝突処理
	/// </summary>
	void CheckAllCollisions();

private:
	/// <summary>
	/// ペアの衝突確認（円同士）
	/// </summary>
	/// <param name="colliderA"></param>
	/// <param name="colliderB"></param>
	void CheckCollisionPair(Collider* colliderA, Collider* colliderB);


public:
	CollisionManager() {};
	~CollisionManager() = default;

};


#include "CollisionManager.h"

void CollisionManager::CheckAllCollisions()
{
	// リスト内のペアを総当たり
	std::list<Collider*>::iterator itrA = colliders_.begin();
	for (; itrA != colliders_.end(); ++itrA) {
		Collider* colliderA = *itrA;

		// イテレータBはイテレータAの次の要素から回す（重複判定を回避）
		std::list<Collider*>::iterator itrB = itrA;
		itrB++;

		for (; itrB != colliders_.end(); ++itrB) {
			Collider* colliderB = *itrB;

			CheckCollisionPair(colliderA, colliderB);
		}
	}
}

void CollisionManager::Reset()
{
	// リストの掃除
	colliders_.clear();
}

void CollisionManager::CheckCollisionPair(Collider* colliderA, Collider* colliderB) {
	Vector3 pointA = colliderA->GetWorldPosition();
	Vector3 pointB = colliderB->GetWorldPosition();
	float distance = {
		powf(pointA.x - pointB.x, 2) + powf(pointA.y - pointB.y, 2) + powf(pointA.z - pointB.z, 2) };
	float radius = colliderA->GetterRad() + colliderB->GetterRad();
	// 衝突フィルタリング
	if ((colliderA->GetAttribute() != colliderB->GetMask()) ||
		(colliderB->GetAttribute() != colliderA->GetMask())) {
		return;
	}

	// 交差判定
	if (distance <= radius) {
		//// ColliderAの衝突時のコールバック
		//colliderA->OnCollision();
		//// ColliderBの衝突時のコールバック
		//colliderB->OnCollision();
	}
}
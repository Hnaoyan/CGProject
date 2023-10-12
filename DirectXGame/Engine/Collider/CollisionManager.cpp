#include "CollisionManager.h"

void CollisionManager::ListClear()
{

	colliders_.clear();

}

void CollisionManager::ListRegister(RectangleCollider* collider)
{

	colliders_.push_back(collider);

}

void CollisionManager::CheakAllCollision()
{

	// リスト内のペアを総当たり
	std::list<RectangleCollider*>::iterator itrA = colliders_.begin();
	for (; itrA != colliders_.end(); ++itrA) {
		// イテレータAからコライダーAを取得する
		RectangleCollider* colliderA = *itrA;
		// イテレータBはイテレータAの次の要素から回す(重複判定を回避)
		std::list<RectangleCollider*>::iterator itrB = itrA;
		itrB++;

		for (; itrB != colliders_.end(); ++itrB) {
			// イテレータBからコライダーBを取得する
			RectangleCollider* colliderB = *itrB;

			// ペアの当たり判定
			CheckCollision(colliderA, colliderB);
		}
	}
}

void CollisionManager::CheckCollision(RectangleCollider* colliderA, RectangleCollider* colliderB)
{

	// 衝突フィルタリング
	if (!(colliderA->GetCollisionAttribute() & colliderB->GetCollisionMask()) ||
		!(colliderB->GetCollisionAttribute() & colliderA->GetCollisionMask())) {
		return;
	}

	float colliderATop = colliderA->GetPosition().y + colliderA->GetSize().y / 2.0f;
	float colliderABottom = colliderA->GetPosition().y - colliderA->GetSize().y / 2.0f;
	float colliderARight = colliderA->GetPosition().x + colliderA->GetSize().x / 2.0f;
	float colliderALeft = colliderA->GetPosition().x - colliderA->GetSize().x / 2.0f;

	float colliderBTop = colliderB->GetPosition().y + colliderB->GetSize().y / 2.0f;
	float colliderBBottom = colliderB->GetPosition().y - colliderB->GetSize().y / 2.0f;
	float colliderBRight = colliderB->GetPosition().x + colliderB->GetSize().x / 2.0f;
	float colliderBLeft = colliderB->GetPosition().x - colliderB->GetSize().x / 2.0f;

	if (colliderATop >= colliderBBottom &&
		colliderABottom <= colliderBTop &&
		colliderARight >= colliderBLeft &&
		colliderALeft <= colliderBRight) {

		// 衝突
		colliderA->OnCollision(colliderB->GetCollisionAttribute(), colliderB->GetWorldTransformAddress());
		colliderB->OnCollision(colliderA->GetCollisionAttribute(), colliderA->GetWorldTransformAddress());

	}


}

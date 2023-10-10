#include "CollisionManager.h"
#include "MathCalc.h"
#include <algorithm>
#include <cmath>

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

void CollisionManager::AddList(Collider* collider)
{
	// リストに追加
	colliders_.push_back(collider);
}

void CollisionManager::ResetList()
{
	// リストの掃除
	colliders_.clear();
}

void CollisionManager::CheckCollisionPair(Collider* colliderA, Collider* colliderB) {
	// 衝突フィルタリング
	//if ((colliderA->GetAttribute() != colliderB->GetMask()) ||
	//	(colliderB->GetAttribute() != colliderA->GetMask())) {
	//	return;
	//}

	// 途中抜け
	if (colliderA->GetAttribute() == colliderB->GetAttribute()) {
		return;
	}

	AABB other1;
	other1.min = VectorLib::Subtract(colliderA->GetPosition(), colliderA->GetterRad());
	other1.max = VectorLib::Add(colliderA->GetPosition(), colliderA->GetterRad());
	AABB other2;
	other2.min = VectorLib::Subtract(colliderB->GetPosition(), colliderB->GetterRad());
	other2.max = VectorLib::Add(colliderB->GetPosition(), colliderB->GetterRad());
	// コールバック
	if (IsAABBCollision(other1, other2)) {
		colliderA->OnCollision(colliderB->GetAttribute(), colliderB->GetWorldAddress());
		colliderB->OnCollision(colliderA->GetAttribute(), colliderA->GetWorldAddress());
	}
}

CollisionManager* CollisionManager::GetInstance()
{
	static CollisionManager instance;
	return &instance;
}

bool CollisionManager::IsCollision(const Sphere& sp1, const Sphere& sp2) {
	Sphere sphereDistance = sp1;
	// 2球体の距離を求める
	sphereDistance.center = VectorLib::Subtract(sphereDistance.center, sp2.center);
	float distance = MathCalc::Length(sphereDistance.center);
	// 半径の合計よりも短いか
	if (distance <= sp1.radius + sp2.radius) {
		return true;
	}
	return false;
}

bool CollisionManager::IsCollision(const Sphere& sp1, const Plane& plane) {
	Sphere sphereDistance = sp1;
	// 2球体の距離を求める
	Vector3 planePoint = MathCalc::Normalize(plane.normal);
	planePoint = { planePoint.x * plane.distance,planePoint.y * plane.distance, planePoint.z * plane.distance };
	float distance = fabsf(MathCalc::Dot(plane.normal, VectorLib::Subtract(sp1.center, planePoint)));

	// 半径の合計よりも短いか
	if (distance <= sp1.radius) {
		return true;
	}
	return false;
}

bool CollisionManager::IsCollision(const Segment& segment, const Plane& plane) {
	// 法線と線の内積
	float dot = MathCalc::Dot(plane.normal, segment.diff);

	// 
	if (dot == 0.0f) {
		return false;
	}
	// tを求める
	float t = (plane.distance - MathCalc::Dot(segment.origin, plane.normal)) / dot;

	if (t > 0 && t < 1) {
		return true;
	}
	return false;
}

bool CollisionManager::IsCollision(const Triangle& triangle, const Segment& segment) {

	Vector3 v01 = VectorLib::Subtract(triangle.vertices[1], triangle.vertices[0]);
	Vector3 v12 = VectorLib::Subtract(triangle.vertices[2], triangle.vertices[1]);
	Vector3 v20 = VectorLib::Subtract(triangle.vertices[0], triangle.vertices[2]);

	Vector3 crossNormal = MathCalc::Cross(v01, v12);
	Vector3 Normal = MathCalc::Normalize(crossNormal);
	float crossDis = MathCalc::Dot(Normal, triangle.vertices[0]);

	float dot = MathCalc::Dot(Normal, segment.diff);

	if (dot == 0.0f) {
		return false;
	}

	float t = (crossDis - MathCalc::Dot(segment.origin, Normal)) / dot;
	if (0.0f < t && t < 1.0f) {

		Vector3 point = VectorLib::Add(segment.origin, VectorLib::Multiply(t, segment.diff));

		Vector3 v1p = VectorLib::Subtract(point, triangle.vertices[1]);
		Vector3 v2p = VectorLib::Subtract(point, triangle.vertices[2]);
		Vector3 v0p = VectorLib::Subtract(point, triangle.vertices[0]);

		Vector3 cross01 = MathCalc::Cross(v01, v1p);
		Vector3 cross12 = MathCalc::Cross(v12, v2p);
		Vector3 cross20 = MathCalc::Cross(v20, v0p);

		if (MathCalc::Dot(cross01, Normal) >= 0.0f &&
			MathCalc::Dot(cross12, Normal) >= 0.0f &&
			MathCalc::Dot(cross20, Normal) >= 0.0f) {
			return true;
		}
	}
	return false;
}

bool CollisionManager::IsAABBCollision(const AABB& aabb1, const AABB& aabb2) {
	if ((aabb1.min.x <= aabb2.max.x && aabb1.max.x >= aabb2.min.x) &&	// X軸
		(aabb1.min.y <= aabb2.max.y && aabb1.max.y >= aabb2.min.y) &&	// Y軸
		(aabb1.min.z <= aabb2.max.z && aabb1.max.z >= aabb2.min.z)) {	// Z軸
		return true;
	}
	return false;
}

bool CollisionManager::IsCollision(const AABB& aabb, const Sphere& sphere) {
	// 最近接点を求める
	Vector3 closestPoint{ std::clamp(sphere.center.x,aabb.min.x,aabb.max.x),
		std::clamp(sphere.center.y,aabb.min.y,aabb.max.y),
		std::clamp(sphere.center.z,aabb.min.z,aabb.max.z) };

	// 最近接点と球の中心との距離を求める
	float distance = MathCalc::Length(VectorLib::Subtract(closestPoint, sphere.center));
	// 距離が半径よりも小さければ衝突
	if (distance <= sphere.radius) {
		return true;
	}

	return false;
}

bool CollisionManager::IsCollision(const AABB& aabb, const Segment& segment) {
	//// AABBとの衝突点（貫通点）のtが小さいほう
	//Vector3 txMin = { (aabb.min.x - segment.origin.x) / segment.diff.x,
	//(aabb.min.y - segment.origin.y) / segment.diff.y ,
	//(aabb.min.z - segment.origin.z) / segment.diff.z };

	//Vector3 txMax = { (aabb.max.x - segment.origin.x) / segment.diff.x,
	//(aabb.max.y - segment.origin.y) / segment.diff.y ,
	//(aabb.max.z - segment.origin.z) / segment.diff.z };

	//Vector3 tNear = { std::min(txMin.x,txMax.x),std::min(txMin.y,txMax.y) ,std::min(txMin.z,txMax.z) };
	//Vector3 tFar = { std::max(txMin.x,txMax.x),std::max(txMin.y,txMax.y) ,std::max(txMin.z,txMax.z) };

	//float tmin = std::max(std::max(tNear.x, tNear.y), tNear.z);
	//float tmax = std::min(std::min(tFar.x, tFar.y), tFar.z);

	//if (tmax < 0.0f || tmin > tmax) {
	//	return false;
	//}

	//if (segment.origin.x >= txMin.x && segment.origin.y >= txMin.y && segment.origin.z >= txMin.z &&
	//	segment.origin.x <= txMax.x && segment.origin.y <= txMax.y && segment.origin.z <= txMax.z) {
	//	return true;
	//}

	//if (tmin <= tmax) {
	//	return true;
	//}
	aabb;
	segment;
	return false;
}

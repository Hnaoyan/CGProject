#pragma once
#include "Collider.h"
#include "StructManager.h"
#include "Model.h"

#include <list>
#include <memory>

class CollisionManager
{
public:
	// リスト
	std::list<Collider*> colliders_;
	
	// デバッグ用モデル
	std::unique_ptr<Model> sphereModel_;

public:
	/// <summary>
	/// すべてのオブジェクトの衝突処理
	/// </summary>
	void CheckAllCollisions();

	/// <summary>
	/// リストをリセット
	/// </summary>
	void ResetList();

	/// <summary>
	/// リストに追加
	/// </summary>
	/// <param name="collider"></param>
	void AddList(Collider* collider);


private:
	/// <summary>
	/// ペアの衝突確認（円同士）
	/// </summary>
	/// <param name="colliderA"></param>
	/// <param name="colliderB"></param>
	void CheckCollisionPair(Collider* colliderA, Collider* colliderB);


public:
	static CollisionManager* GetInstance();

	CollisionManager() {};
	~CollisionManager() = default;

public: // サブクラス
#pragma region 構造体サブクラス
	struct Sphere {
		Vector3 center;	// 中心点
		float radius;	// 半径
	};

	struct Line {
		Vector3 origin;	// 始点
		Vector3 diff;	// 終点への差分ベクトル
	};

	struct Ray {
		Vector3 origin;	// 始点
		Vector3 diff;	// 終点への差分ベクトル
	};

	struct Segment {
		Vector3 origin;	// 始点
		Vector3 diff;	// 終点への差分ベクトル
	};

	struct Plane {
		Vector3 normal;	// 法線
		float distance;	// 距離
	};

	struct Triangle {
		Vector3 vertices[3];
	};

	struct AABB {
		Vector3 min;	// 最小点
		Vector3 max;	// 最大点
	};
#pragma endregion

private: // 衝突判定を取得する関数
#pragma region 衝突の判定取得関数
	/// <summary>
	/// 球と球の衝突判定
	/// </summary>
	/// <param name="sp1"></param>
	/// <param name="sp2"></param>
	/// <returns></returns>
	static bool IsCollision(const Sphere& sp1, const Sphere& sp2);

	/// <summary>
	/// 球と平面の衝突判定
	/// </summary>
	/// <param name="sp1"></param>
	/// <param name="plane"></param>
	/// <returns></returns>
	static bool IsCollision(const Sphere& sp1, const Plane& plane);

	/// <summary>
	/// 線と平面の衝突判定
	/// </summary>
	/// <param name="segment"></param>
	/// <param name="plane"></param>
	/// <returns></returns>
	static bool IsCollision(const Segment& segment, const Plane& plane);

	/// <summary>
	/// 線と三角形の衝突判定
	/// </summary>
	/// <param name="triangle"></param>
	/// <param name="segment"></param>
	/// <returns></returns>
	static bool IsCollision(const Triangle& triangle, const Segment& segment);

	/// <summary>
	/// AABB同士の衝突判定
	/// </summary>
	/// <param name="aabb1"></param>
	/// <param name="aabb2"></param>
	/// <returns></returns>
	static bool IsAABBCollision(const AABB& aabb1, const AABB& aabb2);

	/// <summary>
	/// AABBと球の衝突判定
	/// </summary>
	/// <param name="aabb"></param>
	/// <param name="sphere"></param>
	/// <returns></returns>
	static bool IsCollision(const AABB& aabb, const Sphere& sphere);

	/// <summary>
	/// AABBと線の衝突判定
	/// </summary>
	/// <param name="aabb"></param>
	/// <param name="segment"></param>
	/// <returns></returns>
	static bool IsCollision(const AABB& aabb, const Segment& segment);

#pragma endregion

};


#pragma once
#include "StructManager.h"
#include "WorldTransform.h"
#include <cstdint>
#include <functional>

// プレイヤーの識別番号
const uint32_t kCollisionAttributePlayer = 0b1;

// 敵の識別番号
const uint32_t kCollisionAttributeEnemy = 0b1 << 1;

// 地面
const uint32_t kCollisionAttributeGround = 0b1 << 2;

// ゴール
const uint32_t kCollisionAttributeGoal = 0b1 << 3;

class Collider 
{
private:
	// 半径
	float radius_;

	//Vector3 radius_;

	// 衝突属性（自分）
	int collisionAttribute_ = 0xffffffff;
	// 衝突マスク（自分）
	int collisionMark_ = 0xffffffff;
	// 座標
	Vector3 position_ = {};

	// コールバック関数
	std::function<void(uint32_t, WorldTransform*)> function_;

	WorldTransform* worldTransform_;

public:	// 取得・設定
	void SetCollisionAttribute(int attribute) { collisionAttribute_ = attribute; }
	void SetCollisionMask(int mask) { collisionMark_ = mask; }

	int GetAttribute() { return collisionAttribute_; }
	int GetMask() { return collisionMark_; }

	float GetterRad() { return radius_; }
	void SetterRad(float radius) { radius_ = radius; }

	Vector3 GetPosition() { return position_; }
	void SetPosition(Vector3& pos);

	// ワールドトランスフォーム
	WorldTransform* GetWorldAddress() { return worldTransform_; }
	void SetWorldAddress(WorldTransform* worldTransform) { worldTransform_ = worldTransform; }

	/// <summary>
	/// コールバック設定
	/// </summary>
	/// <param name="function">関数</param>
	void SetFunction(std::function<void(uint32_t, WorldTransform*)> function) { function_ = function; }
	
	/// <summary>
	/// コールバックを呼び出す関数
	/// </summary>
	/// <param name="tag"></param>
	/// <param name="position"></param>
	void OnCollision(uint32_t tag, WorldTransform* world);

};

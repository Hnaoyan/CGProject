#pragma once
#include "StructManager.h"
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
	// 衝突属性（自分）
	int collisionAttribute_ = 0xffffffff;
	// 衝突マスク（自分）
	int collisionMark_ = 0xffffffff;
	// 座標
	Vector3 position_ = {};

	// コールバック関数
	std::function<void(uint32_t, Vector3*)> function_;

public:	// 取得・設定
	void SetCollisionAttribute(int attribute) { collisionAttribute_ = attribute; }
	void SetCollisionMask(int mask) { collisionMark_ = mask; }

	int GetAttribute() { return collisionAttribute_; }
	int GetMask() { return collisionMark_; }

	float GetterRad() { return radius_; }
	Vector3 GetPosition() { return position_; }
	void SetterRad(float radius) { radius_ = radius; }
	void SetPosition(Vector3& pos);
	/// <summary>
	/// コールバック設定
	/// </summary>
	/// <param name="function">関数</param>
	void SetFunction(std::function<void(uint32_t, Vector3*)> function) { function_ = function; }
	
	void OnCollision(uint32_t tag, Vector3* position);

};

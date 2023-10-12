#pragma once
#include <cstdint>

// プレイヤーの識別番号
const uint32_t kCollisionAttributePlayer = 0b1;

// 敵の識別番号
const uint32_t kCollisionAttributeEnemy = 0b1 << 1;

class Collider 
{
private:
	// 半径
	float radius_;
	// 衝突属性（自分）
	int collisionAttribute_ = 0xffffffff;
	// 衝突マスク（自分）
	int collisionMark_ = 0xffffffff;

public:
	// 半径取得
	float GetterRad() { return radius_; }
	void SetterRad(float radius) { radius_ = radius; }
	// 衝突時に呼ばれる関数
	virtual void OnCollision() {};
	//virtual Vector3 GetWorldPosition() = 0;

public:	// 取得・設定
	virtual void SetCollisionAttribute(int attribute) { collisionAttribute_ = attribute; }
	virtual void SetCollisionMask(int mask) { collisionMark_ = mask; }

	virtual int GetAttribute() { return collisionAttribute_; }
	virtual int GetMask() { return collisionMark_; }
};

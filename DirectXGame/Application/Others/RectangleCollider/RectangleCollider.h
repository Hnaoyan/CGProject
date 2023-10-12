#pragma once
#include <StructManager.h>
#include <cstdint>
#include <functional>
#include "WorldTransform.h"

// 衝突属性
enum CollisionAttribute
{
	player = 0b1, // プレイヤーの識別番号
	blockScaffold = 0b1 << 1, // ブロックの識別番号kScaffold
	blockScaffoldColor = 0b1 << 2, // ブロックの識別番号kScaffoldColor
	blockPlayerAttack = 0b1 << 3, // ブロックの識別番号kPlayerAttack
	blockEnemyAttack = 0b1 << 4, // ブロックの識別番号kEnemyAttack
	bossEnemy = 0b1 << 5, // ボスエネミーの識別番号
};

/// <summary>
/// 矩形コライダー
/// </summary>
class RectangleCollider
{

public: // メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(WorldTransform* worldTransfrom, Vector2 size);

	/// <summary>
	/// 更新
	/// </summary>
	void Update(Vector2 position);

	/// <summary>
	/// 衝突処理
	/// </summary>
	/// <param name="collisonObj">相手の衝突属性</param>
	/// <param name="worldTransfrom">ワールドトランスフォーム</param>
	void OnCollision(uint32_t collisonObj, WorldTransform* worldTransfrom);

public: // アクセッサ

	/// <summary>
	/// 衝突属性(自分)セッター
	/// </summary>
	void SetCollisionAttribute(uint32_t collisionAttribute) { collisionAttribute_ = collisionAttribute; }

	/// <summary>
	/// 衝突属性(自分)ゲッター
	/// </summary>
	uint32_t GetCollisionAttribute() { return collisionAttribute_; }

	/// <summary>
	/// 衝突マスク(相手)セッター
	/// </summary>
	void SetCollisionMask(uint32_t collisionMask) { collisionMask_ = collisionMask; }

	/// <summary>
	/// 衝突マスク(相手)ゲッター
	/// </summary>
	uint32_t GetCollisionMask() { return collisionMask_; }

	/// <summary>
	/// コールバックセッター
	/// </summary>
	/// <param name="function">関数</param>
	void SetFunction(std::function<void(uint32_t, WorldTransform*)> function) { function_ = function; }

	/// <summary>
	/// 位置ゲッター
	/// </summary>
	/// <returns></returns>
	Vector2 GetPosition() { return position_; }

	/// <summary>
	/// サイズゲッター
	/// </summary>
	/// <returns></returns>
	Vector2 GetSize() { return size_; }

	/// <summary>
	/// ワールドトランスフォームセッター
	/// </summary>
	/// <param name="worldTransform"></param>
	void SetWorldTransform(WorldTransform* worldTransform) { worldTransform_ = worldTransform; }

	/// <summary>
	/// ワールドトランスフォームゲッター
	/// </summary>
	/// <param name="worldTransform"></param>
	WorldTransform* GetWorldTransformAddress() { return worldTransform_; }

private: // メンバ変数

	// 位置
	Vector2 position_;

	// サイズ
	Vector2 size_;

	// 衝突属性(自分)
	uint32_t collisionAttribute_ = 0xffffffff;

	// 衝突マスク(相手)
	uint32_t collisionMask_ = 0xffffffff;

	// コールバック
	std::function<void(uint32_t, WorldTransform*)> function_;

	// ワールドトランスフォーム
	WorldTransform* worldTransform_;

};


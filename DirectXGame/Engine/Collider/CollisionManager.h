#pragma once
#include <list>
#include <StructManager.h>

#include "Application/Others/RectangleCollider/RectangleCollider.h"

/// <summary>
/// コリジョンマネージャー
/// </summary>
class CollisionManager
{
public: // メンバ関数
	/// <summary>
	/// リストクリア関数
	/// </summary>
	void ListClear();

	/// <summary>
	/// リストにコライダーを登録する関数
	/// </summary>
	/// <param name="collider">登録するコライダー</param>
	void ListRegister(RectangleCollider* collider);

	/// <summary>
	/// 全ての衝突判定を検証する関数
	/// </summary>
	void CheakAllCollision();

private: // メンバ関数

	/// <summary>
	/// 衝突判定検証
	/// </summary>
	void CheckCollision(RectangleCollider* colliderA, RectangleCollider* colliderB);

private: // メンバ変数

	// コライダーリスト
	std::list<RectangleCollider*> colliders_;

};


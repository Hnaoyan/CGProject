#pragma once
#include "BaseCharacter.h"
#include "Input.h"

#include <optional>
#include <functional>

class Player : public BaseCharacter
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model"></param>
	void Initialize(Model* model) override;

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name="viewProjection"></param>
	void Draw(const ViewProjection& viewProjection) override;

	/// <summary>
	/// 衝突コールバック
	/// </summary>
	void OnCollision(uint32_t tag, WorldTransform* world);

	/// <summary>
	/// ブロックと衝突時
	/// </summary>
	void OnCollisionObject();

	/// <summary>
	/// 座標取得
	/// </summary>
	/// <returns></returns>
	Vector3 GetWorldPosition();

	/// <summary>
	/// WorldTransformの取得
	/// </summary>
	/// <returns></returns>
	WorldTransform* GetWorldTransform() { return &worldTransform_; }

	void SetViewProjection(const ViewProjection* viewProjection) {
		viewProjection_ = viewProjection;
	}
public:
	/// <summary>
	/// 死亡時のリスタート関数（外部で呼び出す予定
	/// </summary>
	/// <param name="startPoint"></param>
	void DeadToRestart(const Vector3& startPoint);
	
	bool GetIsDead() { return isDead_; }

private:

	/// <summary>
	/// 移動の更新
	/// </summary>
	void Move();

	/// <summary>
	/// ジャンプの更新
	/// </summary>
	void Jump();

	/// <summary>
	/// 落下の更新
	/// </summary>
	void Fall();

	/// <summary>
	/// 着地時の処理
	/// </summary>
	void Ground();

private: // システム系

	Input* input_ = nullptr;

	bool isJump_ = false;

	bool isLand_ = false;

	bool isParent_ = false;
	
	float jumpPower_ = 0.75f;

	float lerp_t_ = 0;

	// カメラのビュープロジェクション
	const ViewProjection* viewProjection_ = nullptr;

};


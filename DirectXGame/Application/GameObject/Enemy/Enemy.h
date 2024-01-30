#pragma once
#include "BaseCharacter.h"
#include "Input.h"
#include "Collider.h"

#include <optional>
#include <functional>

class Enemy
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model"></param>
	void Initialize(Model* model, Model* propModel);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name="viewProjection"></param>
	void Draw(const ViewProjection& viewProjection);

public:

	WorldTransform* GetTargetAddress() { return &worldTransform_; }

	Vector3 GetWorldPosition() {
		return Vector3(worldTransform_.matWorld_.m[3][0], worldTransform_.matWorld_.m[3][1], worldTransform_.matWorld_.m[3][2]);
	}

	Vector3 GetVelocity() { return velocity_; }

	void SetPosition(const Vector3& position) { worldTransform_.translation_ = position; }

	bool GetIsDead() { return isDead_; }

	Collider* GetCollider() { return &collider_; }

	bool GetIsHit() { return isOnHit_; }

	void SetIsHit(bool isHit) { isOnHit_ = isHit; }

	void Animation();

private: //　アニメーション系
	// 浮遊ギミックの媒介変数
	float floatingParameter_ = 0.0f;

	/// 浮遊の振幅<m>
	float floatingWidth = 0.25f;

	/// 浮遊移動のサイクル<frame>
	int period = 60;

private:
	enum MoveType {
		kOne,
		kTwo,
		kThree,
		kFour,
	};

	Model* model_ = nullptr;
	Model* propModel_ = nullptr;

	Collider collider_;

	// ワールドトランスフォーム
	WorldTransform worldTransform_;
	WorldTransform bodyTransform_;
	WorldTransform propTransform_;

	uint32_t texture_ = 0u;

	Vector3 velocity_ = {};

	float radius_ = 0;
	int invisibleTimer_;

	int moveCount_ = 0;

	int moveType_ = 0;

	bool isDead_ = false;
	bool isOnHit_ = false;

	float SpeedDelta(float moveValue) { return moveValue * (1.0f / 60.0f); }


	void ImGuiWidget();

	void OnCollision(uint32_t tag, WorldTransform* world);

	void TransformInitialize();

};


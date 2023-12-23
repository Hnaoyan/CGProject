#pragma once
#include "BaseCharacter.h"
#include "Enemy/EnemyManager.h"
#include "Missile/MissileManager.h"
#include "Input.h"

#include <optional>
#include <functional>

class Player
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model"></param>
	void Initialize(Model* model);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name="viewProjection"></param>
	void Draw(const ViewProjection& viewProjection);

	void InputUpdate();

public:

	WorldTransform* GetTargetAddress() { return &worldTransform_; }
	Vector3 GetWorldPosition() {
		return Vector3(worldTransform_.matWorld_.m[3][0], worldTransform_.matWorld_.m[3][1], worldTransform_.matWorld_.m[3][2]); }

	void SetEnemyManager(EnemyManager* ptr) { enemyPtr_ = ptr; }
	void SetMissileManager(MissileManager* ptr) { missileManager_ = ptr; }

private:

	Model* model_;
	WorldTransform worldTransform_;

	float SpeedDelta(float moveValue) { return moveValue * (1.0f / 60.0f); }

	EnemyManager* enemyPtr_ = nullptr;
	MissileManager* missileManager_ = nullptr;

	bool isFire_ = false;
	int coolTime_ = 0;

	Vector3 missileDirect = {};
};


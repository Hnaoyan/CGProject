#pragma once
#include "BaseCharacter.h"
#include "Input.h"

#include <optional>
#include <functional>

class Enemy
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

public:

	WorldTransform* GetTargetAddress() { return &worldTransform_; }
	Vector3 GetWorldPosition() {
		return Vector3(worldTransform_.matWorld_.m[3][0], worldTransform_.matWorld_.m[3][1], worldTransform_.matWorld_.m[3][2]);
	}

	void SetPosition(const Vector3& position) { worldTransform_.translation_ = position; }


private:

	Model* model_;
	WorldTransform worldTransform_;
	uint32_t texture_ = 0u;

	float SpeedDelta(float moveValue) { return moveValue * (1.0f / 60.0f); }

};


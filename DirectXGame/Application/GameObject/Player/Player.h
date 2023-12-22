#pragma once
#include "BaseCharacter.h"
#include "Weapon.h"
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

public:

	WorldTransform* GetTargetAddress() { return &worldTransform_; }

private:

	Model* model_;
	WorldTransform worldTransform_;

	float SpeedDelta(float moveValue) { return moveValue * (1.0f / 60.0f); }

};


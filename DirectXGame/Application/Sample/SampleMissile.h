#pragma once
#include "Input.h"
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

class SampleMissile
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

	void InitSetting(Vector3 position, WorldTransform* target) {
		worldTransform_.translation_ = position;
		worldTransform_.UpdateMatrix();

		targetWorld_ = target;
	}

	WorldTransform* GetWorldTransform() { return &worldTransform_; }

private:

	Model* model_ = nullptr;
	WorldTransform* targetWorld_ = nullptr;
	WorldTransform worldTransform_;

	// 方向ベクトル
	Vector3 direction_ = {};
	// 速度ベクトル
	Vector3 velocity_ = {};
	// 加速度
	Vector3 acceleration_ = {};

};


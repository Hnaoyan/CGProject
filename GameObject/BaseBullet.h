#pragma once
#include "Model.h"
#include "ViewProjection.h"
#include "StructManager.h"
#include "WorldTransform.h"

// 基底クラス <Bullet>
class BaseBullet
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model"></param>
	/// <param name="position"></param>
	/// <param name="velocity"></param>
	virtual void Initialize(Model* model, const Vector3& position, const Vector3& velocity);

	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection"></param>
	virtual void Draw(const ViewProjection& viewProjection);

	/// <summary>
	/// 平行移動成分
	/// </summary>
	/// <returns></returns>
	Vector3 GetWorldPosition();

	void SetRotate(Vector3& rotate) { worldTransform_.rotation_ = rotate; }

	void OnCollision();

	/// <summary>
	/// 半径
	/// </summary>
	const float radius = 10.0f;

	bool IsDead() { return isDead_; }

protected:
	// 座標系
	WorldTransform worldTransform_;
	// モデル
	Model* model_;
	// 移動速度
	Vector3 velocity_;
	// 生きているか
	bool isDead_ = false;
	// デスタイマー
	int32_t deathTimer_ = 0;
};

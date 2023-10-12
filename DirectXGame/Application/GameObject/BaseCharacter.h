#pragma once
#include <memory>

#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "StructManager.h"

#include "Collider.h"


class BaseCharacter 
{
protected:
	/// <summary>
	/// 半径
	/// </summary>
	float radius_ = 0;
	/// <summary>
	/// キャラのモデル
	/// </summary>
	Model* model_;
	/// <summary>
	/// 座標系
	/// </summary>
	WorldTransform worldTransform_;

	Collider* collider_;

public:	// デフォルト関数
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model"></param>
	virtual void Initialize(Model* model);

	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name="viewProjection"></param>
	virtual void Draw(const ViewProjection& viewProjection);

public:	// コライダー関数
	/// <summary>
	/// 衝突判定の半径
	/// </summary>
	/// <returns></returns>
	virtual float GetRadius() { return radius_; }

	/// <summary>
	/// 衝突判定の半径設定
	/// </summary>
	/// <param name="radius"></param>
	virtual void SetRadius(float radius) { radius_ = radius; }

	/// <summary>
	/// 衝突時の処理
	/// </summary>
	virtual void OnCollision();

	const WorldTransform GetWorldTransform() { return worldTransform_; }

};

#pragma once
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Collider.h"

class Goal : public Collider
{
private:
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

	uint32_t textureHandle_ = 0u;

public:	// デフォルト関数
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

	/// <summary>
	/// 衝突コールバック
	/// </summary>
	void OnCollision(uint32_t tag, WorldTransform* targetWorldTransform);
};


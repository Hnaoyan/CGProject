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
	/// 死亡フラグ
	/// </summary>
	bool isDead_ = false;
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
	/// <summary>
	/// 速さ
	/// </summary>
	Vector3 velocity_;
	/// <summary>
	/// コライダー
	/// </summary>
	Collider collider_;

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

	/// <summary>
	/// 死亡フラグの取得
	/// </summary>
	/// <returns></returns>
	bool GetIsDead() { return isDead_; }

	/// <summary>
	/// コライダーの取得
	/// </summary>
	/// <returns></returns>
	Collider* GetCollider() { return &collider_; }

private:

	void ColliderSetting();

public:

	void SetPosition(const Vector3& position) { worldTransform_.translation_ = position; }

};

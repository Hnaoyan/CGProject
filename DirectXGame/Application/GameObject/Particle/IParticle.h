#pragma once
#include "StructManager.h"
#include "Model.h"
#include <memory>

class IParticle
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model"></param>
	/// <param name="texture"></param>
	void Initialize(Model* model,uint32_t texture);
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection"></param>
	void Draw(ViewProjection& viewProjection);

public: // 設定・取得
	/// <summary>
	/// フラグの取得
	/// </summary>
	bool IsDead() { return isDead_; }
	/// <summary>
	/// パーティクルの消える時間を設定
	/// </summary>
	/// <param name="count"></param>
	void SetFadeTimer(int count) { fadeTime_ = count; }
	/// <summary>
	/// 座標設定
	/// </summary>
	/// <param name="pos"></param>
	void SetPosition(const Vector3& position) {
		worldTransform_.translation_ = position;
		worldTransform_.UpdateMatrix();
	}

	void SetBillBoard(const ViewProjection* viewProjection) {
		worldTransform_.BillBoardSetting(viewProjection);
	}

	void SetVelocity(const Vector3& velocity) {
		velocity_ = velocity;
	}

private:
	Model* model_;
	WorldTransform worldTransform_;
	Vector3 velocity_;

	uint32_t texture_ = 0u;

	bool isDead_ = false;

	float radius_;

	float ease_t_ = 0;

	// 消える時間
	int fadeTime_ = 0;
	// 経過時間
	int timeElapsed_ = 0;
};


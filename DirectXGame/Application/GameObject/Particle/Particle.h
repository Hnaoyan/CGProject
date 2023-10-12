#pragma once
#include "StructManager.h"
#include "ViewProjection.h"
#include "Model.h"
#include <memory>

class Particle
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model"></param>
	/// <param name="texture"></param>
	virtual void Initialize(Model* model,uint32_t texture);

	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name="viewProjection"></param>
	virtual void Draw(ViewProjection& viewProjection);

public: // 設定・取得
	/// <summary>
	/// フラグの取得
	/// </summary>
	bool IsDead() { return isDead_; }
	/// <summary>
	/// パーティクルの消える時間を設定
	/// </summary>
	/// <param name="count"></param>
	void SetFadeTimer(int count) { fadeTimer_ = count; }
	/// <summary>
	/// 座標設定
	/// </summary>
	/// <param name="pos"></param>
	void SetPosition(Vector3& pos) { worldTransform_.translation_ = pos; }
	/// <summary>
	/// 速さ設定
	/// </summary>
	/// <param name="velo"></param>
	void SetVelocity(Vector3& velo) { velocity_ = velo; }
	/// <summary>
	/// ビルボードの設定
	/// </summary>
	/// <param name="view"></param>
	void SetBillBoard(ViewProjection* view) { worldTransform_.BillBoardSetting(view, true); }
protected:
	// モデル
	Model* model_;
	// ワールドトランスフォーム（座標）
	WorldTransform worldTransform_;
	// 速さ
	Vector3 velocity_;

	Vector3 scale_;

	// テクスチャ
	uint32_t texture_ = 0u;
	// 削除フラグ
	bool isDead_ = false;
	// 消えるカウント
	int fadeTimer_ = 60;
	// 経過時間
	int timeElapsed_ = 0;
};

//class EnemyHitParticle : public Particle {
//
//};
//
//class JumpParticle : public Particle {
//
//};
//
//class BlockHitParticle : public Particle {
//
//};
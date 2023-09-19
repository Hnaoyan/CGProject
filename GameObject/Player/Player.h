#pragma once
#include <Model.h>
#include<WorldTransform.h>
#include <ViewProjection.h>
#include<list>
#include <optional>

#include "BaseCharacter.h"
#include "PlayerBullet.h"

#include "Input.h"

class Player : public BaseCharacter
{
public:
	enum class Behavior {
		kRoot,	// 通常状態
		kJump,	// ジャンプ
		kDash,	// ダッシュ
	};

	Behavior behavior_ = Behavior::kRoot;

	std::optional<Behavior> behaviorRequest_ = std::nullopt;

private:
	struct DashInfo {
		bool isDash;
		int coolDownCount;
	};

public:
	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="model"></param>
	void Initialize(Model* model) override;

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name="viewProjection"></param>
	void Draw(const ViewProjection& viewProjection) override;

	void CameraUpdate();

public:
	
	WorldTransform* GetWorldTransformTarget() { return &worldTransform_; }

	void SetViewProjection(const ViewProjection* viewProjection) {
		viewProjection_ = viewProjection;
	}

	void BulletUpdate();

	void Attack();

	void Jump();

	~Player();

	const std::list<PlayerBullet*>& GetBullets() { return bullets_; }

public:
	void BehaviorRootUpdate(){};

	void BehaviorRootInitialize(){};

	void BehaviorWalkUpdate(){};

	void BehaviorWalkInitialize(){};

	void BehaviorJumpUpdate();

	void BehaviorJumpInitialize();

	void SetBulletModel(Model* model) { bulletModel_ = model; }

private:
	
	Model* bulletModel_ = nullptr;

	Input* input_;

	// カメラのビュープロジェクション
	const ViewProjection* viewProjection_ = nullptr;

	// 弾
	std::list<PlayerBullet*> bullets_;

	Vector2 jumpVelocity_{};

	DashInfo dash;

};

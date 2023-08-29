#pragma once
#include <Model.h>
#include<WorldTransform.h>
#include <ViewProjection.h>
#include<list>
#include <optional>

#include "BaseCharacter.h"
#include "PlayerBullet.h"

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

	void Initialize(Model* model) override;

	void Update() override;

	void Draw(const ViewProjection& viewProjection) override;

	void CameraUpdate();

public:
	
	WorldTransform* GetWorldTransformTarget() { return &worldTransform_; }

	void SetViewProjection(const ViewProjection* viewProjection) {
		viewProjection_ = viewProjection;
	}

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

	// カメラのビュープロジェクション
	const ViewProjection* viewProjection_ = nullptr;

	// 弾
	std::list<PlayerBullet*> bullets_;

	Vector2 jumpVelocity_{};

	DashInfo dash;

	int bulletCount = 0;

	int reloadCount = 0;

	bool isReload = 0;

	// 攻撃系
	int coolTime = 0;

	bool isShot = false;

	int Interval = 50;

	
	// ジャンプ系
	int fallTime_ = 0;

	bool isJump_ = false;

	float jumpPower_ = 4.0f;
};

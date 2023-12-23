#pragma once
#include "StructManager.h"
#include "Model.h"
#include "MathCalc.h"
#include "Enemy/Enemy.h"

class IMissile
{
public:

	void Initialize(Model* model);

	void Update();

	void Draw(ViewProjection& viewProjection);

public:
	Vector3 GetWorldPosition() {
		return Vector3(worldTransform_.matWorld_.m[3][0], worldTransform_.matWorld_.m[3][1], worldTransform_.matWorld_.m[3][2]);
	}

	void SetPosition(const Vector3& position) { worldTransform_.translation_ = position; }
	void SetDirection(const Vector3& direct) { direction_ = MathCalc::Normalize(direct); }
	void SetBulletSpeed(float speed) { kBulletSpeed_ = speed; }

	void SetTarget(Enemy* targetPtr) { target_ = targetPtr; }

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;

	Enemy* target_ = nullptr;

private:
	Vector3 velocity_;
	Vector3 direction_;
	float kBulletSpeed_ = 5.0f;
	float frame_ = 0;
	int guidedTime_ = 0;
};


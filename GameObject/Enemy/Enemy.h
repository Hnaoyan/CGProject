#pragma once
#include "BaseCharacter.h"

class Enemy : public BaseCharacter
{
public:
	void Initialize(Model* model) override;

	void Update() override;

	void Draw(const ViewProjection& viewProjection) override;

	void OnCollision() override;

	bool IsDead() const { return info_.isDead_; }

	void SetPosition(Vector3 pos) { worldTransform_.translation_ = pos; }

public:

	struct EnemyInfo {
		bool isHit;
		bool isDead_;
		int hp;
		int invisibleCount;
	};

private:

	EnemyInfo info_;

	/// <summary>
	/// 無敵時間
	/// </summary>
	int InvisibleTime = 300;

};

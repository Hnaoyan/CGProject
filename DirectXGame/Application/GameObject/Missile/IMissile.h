#pragma once
#include "StructManager.h"
#include "Model.h"
#include "MathCalc.h"
#include "Enemy/Enemy.h"
#include "Collider.h"
#include "Particle/ParticleManager.h"

/// <summary>
/// ミサイルクラス
/// </summary>
class IMissile
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model"></param>
	/// <param name="position"></param>
	void Initialize(Model* model, const Vector3& position);
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection"></param>
	void Draw(ViewProjection& viewProjection);

private:
	Collider collider_;

	ParticleManager* manager_ = nullptr;

public: // アクセッサ・初期化
	/// <summary>
	/// ワールド座標
	/// </summary>
	/// <returns></returns>
	Vector3 GetWorldPosition() {
		return Vector3(worldTransform_.matWorld_.m[3][0], worldTransform_.matWorld_.m[3][1], worldTransform_.matWorld_.m[3][2]);
	}

	Collider* GetCollider() { return &collider_; }

	bool GetIsDead() { return isDead_; }

	void SetPosition(const Vector3& position) { worldTransform_.translation_ = position; }
	void SetDirection(const Vector3& direct) { direction_ = MathCalc::Normalize(direct); }
	void SetBulletSpeed(float speed) {
		kBulletSpeed_ = speed;
		velocity_ = direction_ * speed;
	}
	void SetTarget(Enemy* targetPtr) { target_ = targetPtr; }
	void SetTargetPosition(const Vector3& position) { targetPosition_ = position; }

	void SetType(int type) { type_ = type; }

	void InitMoveParameter(const Vector3& direct, float speed);

	void SettingParameter(float lerpRad, float damping) {
		lerpRad_ = lerpRad;
		damping_ = damping;
	}

	void SetParticle(ParticleManager* manager) { manager_ = manager; }

private: // 内部の固有処理
	/// <summary>
	/// スラープによるデフォルト追尾
	/// </summary>
	void SlerpUpdate();
	/// <summary>
	/// 板野サーカス用の追尾
	/// </summary>
	void ItanoUpdate();

	void TrackingMissileV1();

	void TrackingMissileV2();

	/// <summary>
	/// 比例航法の追尾
	/// </summary>
	void ProportionaMissile();

	/// <summary>
	/// 遠心力利用
	/// </summary>
	void ProportionalV4();

	/// <summary>
	/// 誘導率
	/// </summary>
	void ProportionalV6();

	/// <summary>
	/// 非完全誘導
	/// </summary>
	void ProportionalV7();

	Vector3 GetDeltaTimeVelocity();

private:
	/// <summary>
	/// ホーミング処理の管理
	/// </summary>
	void HomingUpdate();


	void OnCollision(uint32_t tag, WorldTransform* world) {
		isDead_ = true;
		tag, world;
	}

	bool isDead_ = false;
	bool isDelay_ = false;

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;

	Enemy* target_ = nullptr;

private:
	Vector3 velocity_;
	Vector3 direction_;
	Vector3 acceleration_;

	Vector3 targetPosition_;
	Vector3 toEnemy_;

	float kBulletSpeed_ = 5.0f;
	int guidedTime_ = 0;
	float p_ = 0;
	int type_ = 0;
	int coolTime_ = 3;
	int deathCount_ = 0;

	int cancelCount_ = 0;
private:
	float lerpRad_ = 5.0f;
	float damping_ = 0.1f;
	
	float slerp_t_ = 0;
};


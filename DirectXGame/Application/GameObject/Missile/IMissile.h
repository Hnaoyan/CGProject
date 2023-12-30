#pragma once
#include "StructManager.h"
#include "Model.h"
#include "MathCalc.h"
#include "Enemy/Enemy.h"

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

public: // アクセッサ・初期化
	/// <summary>
	/// ワールド座標
	/// </summary>
	/// <returns></returns>
	Vector3 GetWorldPosition() {
		return Vector3(worldTransform_.matWorld_.m[3][0], worldTransform_.matWorld_.m[3][1], worldTransform_.matWorld_.m[3][2]);
	}
	void SetPosition(const Vector3& position) { worldTransform_.translation_ = position; }
	void SetDirection(const Vector3& direct) { direction_ = MathCalc::Normalize(direct); }
	void SetBulletSpeed(float speed) {
		kBulletSpeed_ = speed;
		velocity_ = direction_ * speed;
	}
	void SetTarget(Enemy* targetPtr) { target_ = targetPtr; }

	void SetType(int type) { type_ = type; }

	void InitMoveParameter(const Vector3& direct, float speed);

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
	/// <summary>
	/// ホーミング処理の管理
	/// </summary>
	void HomingUpdate();
	Vector3 GetDeltaTimeVelocity();

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;

	Enemy* target_ = nullptr;

private:
	Vector3 velocity_;
	Vector3 direction_;
	Vector3 acceleration_;
	float kBulletSpeed_ = 5.0f;
	float frame_ = 0;
	int guidedTime_ = 100;
	float p_ = 0;
	int type_ = 0;
	float coolTime_ = 100;

private:
	float lerpRad_ = 5.0f;
	float damping_ = 0.1f;
};


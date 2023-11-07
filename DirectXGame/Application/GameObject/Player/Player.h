#pragma once
#include "BaseCharacter.h"
#include "Input.h"

#include <optional>
#include <functional>

class Player : public BaseCharacter
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model"></param>
	void Initialize(const std::vector<Model*>& models);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name="viewProjection"></param>
	void Draw(const ViewProjection& viewProjection) override;

	/// <summary>
	/// 衝突コールバック
	/// </summary>
	void OnCollision(uint32_t tag, WorldTransform* targetWorldTransform);

	/// <summary>
	/// 設定
	/// </summary>
	void Setting();

	/// <summary>
	/// 座標取得
	/// </summary>
	/// <returns></returns>
	Vector3 GetWorldPosition();

	/// <summary>
	/// WorldTransformの取得
	/// </summary>
	/// <returns></returns>
	WorldTransform* GetWorldTransform() { return &worldTransform_; }

	/// <summary>
	/// viewの設定
	/// </summary>
	/// <param name="viewProjection"></param>
	void SetViewProjection(const ViewProjection* viewProjection) {
		viewProjection_ = viewProjection;
	}
private:
	// モデルデータ配列
	std::vector<Model*> models_;
	// モデルデータ識別
	enum ModelPart {
		BODY,
		//HEAD,
		L_ARM,
		R_ARM,
		WEAPON,
	};

	WorldTransform worldTransformBody_;
	WorldTransform worldTransformL_Arm_;
	WorldTransform worldTransformR_Arm_;
	WorldTransform worldTransformWeapon_;


public:
	/// <summary>
	/// 死亡時のリスタート関数（外部で呼び出す予定
	/// </summary>
	/// <param name="startPoint"></param>
	void DeadToRestart(const Vector3& startPoint);
	
	/// <summary>
	/// 死亡時のフラグ取得
	/// </summary>
	/// <returns></returns>
	bool GetIsDead() { return isDead_; }

private:

	/// <summary>
	/// 移動の更新
	/// </summary>
	void ProcessMovement();

	/// <summary>
	/// ジャンプの更新
	/// </summary>
	void ProcessJump();

	/// <summary>
	/// 落下の更新
	/// </summary>
	void Fall();

	/// <summary>
	/// 着地時の処理
	/// </summary>
	void Ground();

	/// <summary>
	/// 調整項目用関数
	/// </summary>
	void ApplyGlobalVariables();

private:
	/// <summary>
	/// 通常状態の初期化
	/// </summary>
	void BehaviorRootInitialize();

	/// <summary>
	/// 通常状態の更新
	/// </summary>
	void BehaviorRootUpdate();

	/// <summary>
	/// ダッシュの初期化
	/// </summary>
	void BehaviorDashInitialize();

	/// <summary>
	/// ダッシュの更新
	/// </summary>
	void BehaviorDashUpdate();

	/// <summary>
	/// 攻撃の初期化
	/// </summary>
	void BehaviorAttackInitialize();

	/// <summary>
	/// 攻撃の更新
	/// </summary>
	void BehaviorAttackUpdate();

private:
	/// <summary>
	/// 状態管理
	/// </summary>
	enum class Behavior {
		kRoot,
		kAttack,
		kDash,
	};

	/// <summary>
	/// 状態の変数
	/// </summary>
	Behavior behavior_ = Behavior::kRoot;

	/// <summary>
	/// 次の動きのリクエスト
	/// </summary>
	std::optional<Behavior> behaviorRequest_ = std::nullopt;

private: // ダッシュ

	struct WorkDash {
		uint32_t dashParameter_ = 0;
	};

	WorkDash workDash_;

private: // 攻撃

	enum class Attack {
		kDown,
		kStop,
	};

	Attack attackState_;

	struct WorkAttack {
		uint32_t maxStunDuration_ = 60;
		uint32_t stunDuration_ = 0;
	};

	WorkAttack workAttack_;

private: // システム系

	Input* input_ = nullptr;

	bool isJump_ = false;

	bool isLand_ = false;
	
	float jumpPower_ = 0.75f;

	float rotateLerp_t_ = 0;

	float destinationAngleY_ = 0;

	// カメラのビュープロジェクション
	const ViewProjection* viewProjection_ = nullptr;

};


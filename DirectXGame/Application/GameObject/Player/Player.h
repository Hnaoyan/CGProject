#pragma once
#include "BaseCharacter.h"
#include "Weapon.h"
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
	Vector3 GetWorldPosition() {
		Vector3 worldPos = { worldTransform_.matWorld_.m[3][0],worldTransform_.matWorld_.m[3][1],worldTransform_.matWorld_.m[3][2] };
		return worldPos;
	}

	/// <summary>
	/// WorldTransformの取得
	/// </summary>
	/// <returns></returns>
	WorldTransform* GetWorldTransform() { return &worldTransform_; }

	Weapon* GetWeapon() { return weapon_.get(); }

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
	WorldTransform worldAttackCollider_;

	std::unique_ptr<Weapon> weapon_;

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

	bool GetIsAttack() { return behavior_ == Behavior::kAttack; }

private:
#pragma region 基本動作
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
#pragma endregion

private:
#pragma region Behaviorごとの関数
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

	void BehaviorJumpInitialize();

	void BehaviorJumpUpdate();

#pragma endregion

	void CollisionUpdate();

private:
	/// <summary>
	/// 状態管理
	/// </summary>
	enum class Behavior {
		kRoot,
		kAttack,
		kDash,
		kJump,
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
		int dashTime_ = 0;
	};

	WorkDash workDash_;

private: // 攻撃

	enum class Attack {
		kDown,
		kStop,
	};

	Attack attackState_;

	// 攻撃用ワーク
	struct WorkAttack {
		uint32_t attackParameter_ = 0;
		int32_t comboIndex_ = 0;
		int32_t inComboPhase_ = 0;
		bool comboNext_ = false;

	};

	WorkAttack workAttack_;

	// 攻撃用定数
	struct ConstAttack {
		// 振りかぶりの時間<frame>
		uint32_t anticipationTime_;
		// ための時間<frame>
		uint32_t chargeTime_;
		// 攻撃振りの時間<frame>
		uint32_t swingTime_;
		// 硬直時間<frame>
		uint32_t recoveryTime_;
		// 振りかぶりの移動速さ
		float anticipationSpeed_;
		// ための速さ
		float chargeSpeed_;
		// 攻撃振りの速さ
		float swingSpeed_;
	};

	// コンボの数
	static const int ComboNum = 3;

	int countTimer_ = 0;

	// コンボ定数表
	static const std::array<ConstAttack, ComboNum> kConstAttacks_;

	std::vector<std::function<void()>> attackMotions_;

private: // 攻撃の関数
	/// <summary>
	/// １段目
	/// </summary>
	void AttackCombo1();
	/// <summary>
	/// 2段目
	/// </summary>
	void AttackCombo2();
	/// <summary>
	/// ３段目
	/// </summary>
	void AttackCombo3();

private: // システム系

	Input* input_ = nullptr;

	Vector3 moveDirection_ = {};

	bool isJump_ = false;

	bool isLand_ = false;
	
	float jumpPower_ = 0.75f;

	float rotateLerp_t_ = 0;

	float destinationAngleY_ = 0;

	// カメラのビュープロジェクション
	const ViewProjection* viewProjection_ = nullptr;

	std::unique_ptr<Model> deModel_;
};


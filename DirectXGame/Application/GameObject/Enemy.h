#pragma once
#include "BaseCharacter.h"

class Enemy : public BaseCharacter
{
public:
	/// <summary>
	/// 初期化
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

	void Move();

	void UpdateCollider();

public:

	void SetModel(const std::vector<Model*>& models);

	/// <summary>
	/// 衝突コールバック
	/// </summary>
	void OnCollision(uint32_t tag, WorldTransform* targetWorldTransform);
	/// <summary>
	/// 座標取得
	/// </summary>
	/// <returns></returns>
	Vector3 GetWorldPosition();

	const Vector3& GetWorldPositionTarget() const
	{
		static Vector3 tmp = { worldTransform_.matWorld_.m[3][0],worldTransform_.matWorld_.m[3][1] ,worldTransform_.matWorld_.m[3][2] };
		return tmp;
	}

	WorldTransform GetTransform() const { return worldTransform_; }

	void UpdateFloating();

	void InitializeFloating();

	void MoveInitialize();

	void SetIsDead(bool isDead) { isDead_ = isDead; }

	bool GetIsDead() { return isDead_; }
	
private:
	// モデルデータ配列
	std::vector<Model*> models_;

	WorldTransform worldBody_;
	WorldTransform worldL_arm_;
	WorldTransform worldR_arm_;

	// 浮遊ギミックの媒介変数
	float floatingParameter_ = 0.0f;

	/// 浮遊の振幅<m>
	float floatingWidth = 0.25f;

	/// 浮遊移動のサイクル<frame>
	int period = 60;

	enum ModelPart {
		BODY,
		L_ARM,
		R_ARM,
	};

	bool isLeft_ = false;

	Vector3 velocity_ = {};

};


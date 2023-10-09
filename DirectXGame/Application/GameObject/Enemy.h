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

public:

	void SetModel(const std::vector<Model*>& models);

	/// <summary>
	/// 衝突コールバック
	/// </summary>
	void OnCollision();
	/// <summary>
	/// 座標取得
	/// </summary>
	/// <returns></returns>
	Vector3 GetWorldPosition();

	void UpdateFloating();

	void InitializeFloating();

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
};


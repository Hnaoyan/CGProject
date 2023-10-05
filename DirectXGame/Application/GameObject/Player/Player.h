#pragma once
#include "BaseCharacter.h"

class Player : public BaseCharacter
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

	//void OnCollision() override;
	//Vector3 GetWorldPosition() override;


};


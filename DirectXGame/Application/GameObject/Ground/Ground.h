#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include "Collider.h"

#include <memory>

class Ground
{
public:

	virtual void Initialize(Model* model);

	virtual void Update();

	virtual void Draw(const ViewProjection& viewProjection);

	void SetPosition(const Vector3& position);

	void SetScale(const Vector3& scale);
	/// <summary>
	/// WorldTransformの取得
	/// </summary>
	/// <returns></returns>
	WorldTransform* GetWorldTransform() { return &worldTransform_; }
	/// <summary>
	/// 衝突コールバック
	/// </summary>
	void OnCollision(uint32_t tag, WorldTransform* targetWorldTransform);

	Collider* GetCollider() { return &collider_; }

protected:
	
	WorldTransform worldTransform_;

	Model* model_ = nullptr;

	Collider collider_;

	Vector3 velocity_;
};


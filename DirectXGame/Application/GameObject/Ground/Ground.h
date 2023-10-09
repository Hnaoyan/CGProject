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

	void SetPosition(const Vector3& pos);

protected:
	
	WorldTransform worldTransform_;

	Model* model_ = nullptr;

	Collider collider_;

};

class MoveGround : public Ground 
{
public:
	void Initialize(Model* model) override;

	void Update() override;

	void Draw(const ViewProjection& viewProjection) override;
};

class NormalGround : public Ground 
{
public:
	void Initialize(Model* model) override;

	void Update() override;

	void Draw(const ViewProjection& viewProjection) override;
};
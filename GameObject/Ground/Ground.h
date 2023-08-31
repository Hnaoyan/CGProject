#pragma once
#include "WorldTransform.h"
#include "Model.h"

class Ground 
{
public:

	void Initialize(Model* model);

	void Update();

	void Draw(const ViewProjection& viewProjection);

	float radius_ = 250.0f;

	AABB GetAABB() { return aabb_; }

private:
	
	WorldTransform worldTransform_;

	Model* model_ = nullptr;

	AABB aabb_ = {};

};

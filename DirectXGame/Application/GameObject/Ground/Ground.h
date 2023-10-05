#pragma once
#include "WorldTransform.h"
#include "Model.h"

class Ground 
{
public:

	void Initialize(Model* model);

	void Update();

	void Draw(const ViewProjection& viewProjection);

private:
	
	WorldTransform worldTransform_;

	Model* model_ = nullptr;

};

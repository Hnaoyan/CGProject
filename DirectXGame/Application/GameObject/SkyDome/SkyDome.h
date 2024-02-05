#pragma once
#include "WorldTransform.h"
#include "Model.h"

class SkyDome {
public:

	SkyDome() {};
	~SkyDome() = default;

	void Initialize(Model* model);

	void Update();

	void Draw(const ViewProjection& view);

private:

	WorldTransform world_ = {};

	Model* model_ = nullptr;

};

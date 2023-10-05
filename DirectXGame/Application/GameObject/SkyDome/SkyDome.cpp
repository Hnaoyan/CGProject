#include "SkyDome.h"

void SkyDome::Initialize(Model* model)
{
	world_.Initialize();

	model_ = model;
}

void SkyDome::Update()
{
	world_.UpdateMatrix();
}

void SkyDome::Draw(const ViewProjection& view)
{
	model_->Draw(world_, view);
}

#pragma once
#include "BaseBullet.h"

class PlayerBullet : public BaseBullet
{
public:
	void Initialize(Model* model, const Vector3& position, const Vector3& velocity) override;

	void Update() override;

	void Draw(const ViewProjection& viewProjection) override;

private:


};

#pragma once
#include "../IMissile.h"

class SlerpMissile : public IMissile
{
public:

	void Initialize(Model* model, const Vector3& position) override;

	void Update() override;
	void Draw(ViewProjection& viewProjection) override;

};


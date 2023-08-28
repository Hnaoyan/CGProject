#pragma once
#include "BaseCharacter.h"

class Enemy : public BaseCharacter
{
public:
	void Initialize(Model* model) override;

	void Update() override;

	void Draw(const ViewProjection& viewProjection) override;



};

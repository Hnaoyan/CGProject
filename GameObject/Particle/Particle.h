#pragma once
#include "StructManager.h"

class Particle
{
public:

	void Initialize() {};

	void Update() {};

	void Draw() {};

private:
	Vector2 position_{};
	Vector2 velocity_{};

	bool isDead_ = false;

	float radius_;
};


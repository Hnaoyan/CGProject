#pragma once
#include "StructManager.h"
#include "Particle.h"
#include <list>

class ParticleManager
{
public:

	void Initialize();

	void Update();

	void Draw();

private:

	std::list<Particle*> particles_;

};


#include "ParticleManager.h"

void ParticleManager::Initialize()
{
}

void ParticleManager::Update()
{
	for (Particle* particle : particles_) {
		particle->Update();
	}
	// リストの削除
	particles_.remove_if([](Particle* particle) {
		if (particle->IsDead()) {
			delete particle;
			return true;
		}
		return false;
	});


}

void ParticleManager::Draw()
{
}

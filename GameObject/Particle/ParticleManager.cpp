#include "ParticleManager.h"
#include "TextureManager.h"

ParticleManager* ParticleManager::GetInstance()
{
	static ParticleManager instance;
	return &instance;
}

void ParticleManager::Initialize()
{
	model_.reset(Model::Create());
	texture_ = TextureManager::Load("white1x1.png");
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

void ParticleManager::Draw(ViewProjection& viewProjection)
{
	for (Particle* particle : particles_) {
		particle->Draw(viewProjection);
	}
}

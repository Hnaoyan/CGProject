#include "ParticleManager.h"
#include "TextureManager.h"

ParticleManager* ParticleManager::GetInstance()
{
	static ParticleManager instance;
	return &instance;
}

void ParticleManager::Initialize()
{
	model_.reset(Model::CreatePlane());
	texture_ = TextureManager::Load("white1x1.png");
}

void ParticleManager::Update()
{

	// パーティクルの更新
	ParticleUpdate();
}

void ParticleManager::ParticleUpdate()
{
	particles_.remove_if([](IParticle* particle) {
		if (particle->IsDead()) {
			delete particle;
			return true;
		}
		return false;
		});

	for (IParticle* particle : particles_) {
		particle->Update();
	}

}

void ParticleManager::Draw(ViewProjection& viewProjection)
{
	for (IParticle* particle : particles_) {
		particle->Draw(viewProjection);
	}
}

void ParticleManager::AddParticle(const Vector3& position, uint32_t fadeTimer)
{
	IParticle* newParticle = new IParticle();
	newParticle->Initialize(model_.get(), texture_);
	newParticle->SetPosition(position);
	newParticle->SetFadeTimer(fadeTimer);
	newParticle->SetBillBoard(viewPtr_);
	particles_.push_back(newParticle);

}

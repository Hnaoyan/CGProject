#include "ParticleManager.h"
#include "TextureManager.h"

ParticleManager* ParticleManager::GetInstance()
{
	static ParticleManager instance;
	return &instance;
}

void ParticleManager::Initialize()
{
	cubeModel_.reset(Model::Create());
	planeModel_.reset(Model::CreatePlane());
	texture_ = TextureManager::Load("white1x1.png");
}

void ParticleManager::Update()
{

	// パーティクルの更新
	ParticleUpdate();
}

void ParticleManager::ParticleUpdate()
{
	for (IParticle* particle : particles_) {
		particle->Update();
	}
	// リストの削除
	particles_.remove_if([](IParticle* particle) {
		if (particle->IsDead()) {
			delete particle;
			return true;
		}
		return false;
	});
}

void ParticleManager::Draw(ViewProjection& viewProjection)
{
	for (IParticle* particle : particles_) {
		particle->Draw(viewProjection);
	}
}

void ParticleManager::AddParitcle()
{

	IParticle* newParticle = new IParticle();
	uint32_t texture = TextureManager::Load("white1x1.png");
	newParticle->Initialize(planeModel_.get(), texture);
	particles_.push_back(newParticle);

}

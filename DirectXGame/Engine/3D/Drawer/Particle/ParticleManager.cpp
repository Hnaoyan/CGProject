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

	// パーティクルの更新
	ParticleUpdate();
}

void ParticleManager::ParticleUpdate()
{

}

void ParticleManager::Draw(ViewProjection& viewProjection)
{
	viewProjection;
}

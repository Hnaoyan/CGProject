#include "ParticleManager.h"
#include "TextureManager.h"
#include "VectorLib.h"

ParticleManager* ParticleManager::GetInstance()
{
	static ParticleManager instance;
	return &instance;
}

void ParticleManager::Initialize(ViewProjection* view)
{
	view_ = view;
	//model_.reset(Model::CreateFromObj("block", true));
	model_.reset(Model::CreatePlane());
	texture_ = TextureManager::Load("plane/test.png");
}

void ParticleManager::Update()
{

	// パーティクルの更新
	ParticleUpdate();
}

void ParticleManager::ParticleUpdate()
{
	for (Particle* particle : particles_) {
		particle->Update();
	}
	// リストの削除・リリース
	particles_.remove_if([](Particle* particle) {
		if (particle->IsDead()) {
			delete particle;
			// セーフガード
			particle = nullptr;
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

void ParticleManager::RandomRespown(const Vector3& point)
{
	for (int i = 0; i < 5; i++) {
		Vector3 resPos = { (float)(rand() % 5 - 2) + point.x,(float)(rand() % 5 - 2) + point.y ,(float)(rand() % 5 - 2) + point.z };
		Vector3 inverseVect = VectorLib::Subtract(resPos, point);
		float speed = 3.0f;
		float deltaTime = (1.0f / 60.0f) * speed;
		inverseVect = VectorLib::Scaler(inverseVect, deltaTime);
		AddParticle3D(resPos, inverseVect);
	}
}

void ParticleManager::AddParticle2D(Vector3& position, Vector3& velocity)
{
	Particle* newParticle = new Particle();
	newParticle->Initialize(model_.get(), texture_);
	newParticle->SetPosition(position);
	newParticle->SetVelocity(velocity);
	newParticle->SetBillBoard(view_);
	particles_.push_back(newParticle);
}

void ParticleManager::AddParticle3D(Vector3& position, Vector3& velocity)
{
	Particle* newParticle = new Particle();
	newParticle->Initialize(model_.get(), texture_);
	newParticle->SetPosition(position);
	newParticle->SetVelocity(velocity);
	particles_.push_back(newParticle);
}

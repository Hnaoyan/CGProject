#include "GroundManager.h"
#include "CollisionManager.h"

GroundManager* GroundManager::GetInstance()
{
    static GroundManager instance;
    return &instance;
}

void GroundManager::Initialize()
{
	groundModel_.reset(Model::CreateFromObj("Ground", true));
}

void GroundManager::Update()
{
	for (Ground* ground : grounds_) {
		ground->Update();
	}
}

void GroundManager::Draw(const ViewProjection& view)
{
	for (Ground* ground : grounds_) {
		ground->Draw(view);
	}
}

void GroundManager::AddMoveGround(const Vector3& pos, const Vector3& rad, const Vector3& scale)
{
	Ground* newGround = new Ground();
	newGround->Initialize(groundModel_.get());
	newGround->SetPosition(pos);
	newGround->SetScale(scale);
	newGround->GetCollider()->SetterRad(rad);
	newGround->GetCollider()->SetCollisionAttribute(kCollisionAttributeMoveGround);
	grounds_.push_back(newGround);
}

void GroundManager::AddGround(const Vector3& pos, const Vector3& rad, const Vector3& scale)
{
	Ground* newGround = new Ground();
	newGround->Initialize(groundModel_.get());
	newGround->SetPosition(pos);
	newGround->SetScale(scale);
	newGround->GetCollider()->SetterRad(rad);
	newGround->GetCollider()->SetCollisionAttribute(kCollisionAttributeGround);
	grounds_.push_back(newGround);
}


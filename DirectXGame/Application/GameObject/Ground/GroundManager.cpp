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

void GroundManager::AddList()
{
	for (Ground* ground : grounds_) {
		manager_->AddList(ground->GetCollider());
	}
}

void GroundManager::AddGround(const Vector3& pos, const Vector3& rad, const int type)
{
	Ground* newGround = new Ground();
	newGround->Initialize(groundModel_.get());
	newGround->SetPosition(pos);
	newGround->GetCollider()->SetterRad(rad);
	if (type == kCollisionAttributeGround) {
		newGround->GetCollider()->SetCollisionAttribute(type);

	}
	grounds_.push_back(newGround);
}

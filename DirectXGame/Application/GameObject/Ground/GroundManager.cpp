#include "GroundManager.h"
#include "CollisionManager.h"

GroundManager* GroundManager::GetInstance()
{
    static GroundManager instance;
    return &instance;
}

void GroundManager::Initialize()
{
	groundModel_.reset(Model::CreateFromObj("ground", true));
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
	//for (Ground* ground : grounds_) {

	//}
}

void GroundManager::AddGround(const Vector3& pos, const int type)
{
	Ground* newGround = new Ground();
	newGround->Initialize(groundModel_.get());
	newGround->SetPosition(pos);
	type;
	grounds_.push_back(newGround);
}

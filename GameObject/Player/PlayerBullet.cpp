#include "PlayerBullet.h"

void PlayerBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity) 
{

	BaseBullet::Initialize(model, position, velocity);
}

void PlayerBullet::Update() 
{
	BaseBullet::Update();
}

void PlayerBullet::Draw(const ViewProjection& viewProjection)
{
	BaseBullet::Draw(viewProjection);
}

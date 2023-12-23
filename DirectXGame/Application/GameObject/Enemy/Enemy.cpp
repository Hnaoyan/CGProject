#include "Enemy.h"
#include "TextureManager.h"

void Enemy::Initialize(Model* model)
{

	model_ = model;
	worldTransform_.Initialize();
	texture_ = TextureManager::Load("white1x1.png");
}

void Enemy::Update()
{
#ifdef _DEBUG
	Vector3 moveVector{};
	float speedValue = 10.0f;

	XINPUT_STATE joyState;
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		moveVector.x += (float)joyState.Gamepad.sThumbRX / SHRT_MAX * SpeedDelta(speedValue);
		moveVector.y += (float)joyState.Gamepad.sThumbRY / SHRT_MAX * SpeedDelta(speedValue);
	}
	worldTransform_.translation_ += moveVector;
#endif // _DEBUG

	worldTransform_.UpdateMatrix();
}

void Enemy::Draw(const ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, texture_);
}

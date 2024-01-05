#include "Enemy.h"
#include "TextureManager.h"
#include "NLib.h"
#include "imgui.h"

void Enemy::Initialize(Model* model)
{

	model_ = model;
	worldTransform_.Initialize();
	texture_ = TextureManager::Load("white1x1.png");
	velocity_ = { 0,1,1 };
}

void Enemy::Update()
{
#ifdef _DEBUG
	Vector3 moveVector{};
	float speedValue = 10.0f;

	XINPUT_STATE joyState;
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		moveVector.x += (float)joyState.Gamepad.sThumbRX / SHRT_MAX * speedValue;
		moveVector.y += (float)joyState.Gamepad.sThumbRY / SHRT_MAX * speedValue;
		moveVector.z += (float)joyState.Gamepad.sThumbLY / SHRT_MAX * speedValue;
	}

	ImGuiWidget();

#endif // _DEBUG
	//velocity_ = {};

	worldTransform_.translation_ += (velocity_ + moveVector) * NLib::GetDeltaTime(60.0f);

	worldTransform_.UpdateMatrix();
}

void Enemy::Draw(const ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, texture_);
}

void Enemy::ImGuiWidget()
{
	ImGui::Begin("enemy");

	ImGui::DragFloat3("pos", &worldTransform_.translation_.x);

	ImGui::End();

}

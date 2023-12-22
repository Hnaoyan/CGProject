#include "Player.h"
#include "MathCalc.h"
#include "ImGuiManager.h"
#include "GlobalVariables.h"

#include <algorithm>

void Player::Initialize(Model* model)
{
	model_ = model;

	worldTransform_.Initialize();

}

void Player::Update()
{
	Vector3 moveVector{};
	float speedValue = 10.0f;

	XINPUT_STATE joyState;
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		moveVector.x += (float)joyState.Gamepad.sThumbLX / SHRT_MAX * SpeedDelta(speedValue);
		moveVector.y += (float)joyState.Gamepad.sThumbLY / SHRT_MAX * SpeedDelta(speedValue);
	}


	worldTransform_.translation_ += moveVector;
	worldTransform_.translation_.z += 0.1f;
	worldTransform_.UpdateMatrix();
}

void Player::Draw(const ViewProjection& viewProjection)
{

	// 自機描画
	model_->Draw(worldTransform_, viewProjection);

}

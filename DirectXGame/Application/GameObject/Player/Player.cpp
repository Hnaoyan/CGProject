#include "Player.h"
#include "MathCalc.h"
#include "ImGuiManager.h"
#include "GlobalVariables.h"
#include "Missile/MissileManager.h"

#include <algorithm>

void Player::Initialize(Model* model)
{
	model_ = model;

	worldTransform_.Initialize();

}

void Player::Update()
{
	InputUpdate();

	Vector3 moveVector{};
	float speedValue = 10.0f;

	XINPUT_STATE joyState;
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		moveVector.x += (float)joyState.Gamepad.sThumbLX / SHRT_MAX * SpeedDelta(speedValue);
		moveVector.y += (float)joyState.Gamepad.sThumbLY / SHRT_MAX * SpeedDelta(speedValue);
	}


	worldTransform_.translation_ += moveVector;
	//worldTransform_.translation_.z += 0.1f;
	worldTransform_.UpdateMatrix();
}

void Player::Draw(const ViewProjection& viewProjection)
{

	// 自機描画
	model_->Draw(worldTransform_, viewProjection);

}

void Player::InputUpdate()
{
#ifdef _DEBUG

	ImGui::Begin("player");
	if (ImGui::TreeNode("missileInfo")) {
		ImGui::DragFloat3("missileDirect", &missileDirect.x, 0.01f, -1.0f, 1.0f);
		ImGui::TreePop();
	}
	ImGui::End();
#endif // _DEBUG

	// ゲームパッドの状態を得る変数(XINPUT)
	XINPUT_STATE joyState;

	// ジョイスティック状態取得
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER && !isFire_) {
			Vector3 enemyToPlayer = this->enemyPtr_->GetTestWorld() - GetWorldPosition();
			MissileManager::MissileConfig info;
			info = { GetWorldPosition(),enemyToPlayer,0, enemyPtr_->GetTestPtr() };
			missileManager_->AddMissile(info);

			isFire_ = true;
		}
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER && !isFire_) {
			//Vector3 enemyToPlayer = this->enemyPtr_->GetTestWorld() - GetWorldPosition();
			MissileManager::MissileConfig info;
			info = { GetWorldPosition(),missileDirect,0, enemyPtr_->GetTestPtr() };
			missileManager_->AddMissile(info);

			isFire_ = true;
		}
	}

	if (isFire_) {
		coolTime_++;
		if (coolTime_ > 20) {
			coolTime_ = 0;
			isFire_ = false;
		}
	}

}

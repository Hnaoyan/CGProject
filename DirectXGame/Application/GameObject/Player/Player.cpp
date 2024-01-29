#include "Player.h"
#include "MathCalc.h"
#include "ImGuiManager.h"
#include "GlobalVariables.h"
#include "Missile/MissileManager.h"

#include <algorithm>

void Player::Initialize(Model* model)
{
	model_ = model;
	input_ = Input::GetInstance();
	worldTransform_.Initialize();
	worldTransform_.translation_.y = -4.0f;

}

void Player::Update()
{
	InputUpdate();

	Vector3 moveVector{};
	float speedValue = 10.0f;
#ifdef _DEBUG
	if (input_->PressKey(DIK_W)) {
		moveVector.y += SpeedDelta(speedValue);
	}
	if (input_->PressKey(DIK_S)) {
		moveVector.y -= SpeedDelta(speedValue);
	}
	if (input_->PressKey(DIK_A)) {
		moveVector.x -= SpeedDelta(speedValue);
	}
	if (input_->PressKey(DIK_D)) {
		moveVector.x += SpeedDelta(speedValue);
	}
#endif // _DEBUG

	XINPUT_STATE joyState;
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		moveVector.x += (float)joyState.Gamepad.sThumbLX / SHRT_MAX * SpeedDelta(speedValue);
		//moveVector.y += (float)joyState.Gamepad.sThumbLY / SHRT_MAX * SpeedDelta(speedValue);
	}

	//if (worldTransform_.translation_.x + moveVector.x > 10.0f) {
	//	worldTransform_.translation_.x = 9.9f;
	//	moveVector.x = 0;
	//}

	//if (worldTransform_.translation_.x - moveVector.x < -10.0f) {
	//	worldTransform_.translation_.x = -9.9f;
	//	moveVector.x = 0;
	//}

	worldTransform_.translation_ += moveVector;
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
	ImGui::InputFloat3("rotate", &worldTransform_.rotation_.x);

	ImGui::End();


#endif // _DEBUG

	// ゲームパッドの状態を得る変数(XINPUT)
	XINPUT_STATE joyState;

	// ジョイスティック状態取得
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		bool isControler = joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER && !isFire_;
		if (isControler) {
			MissileManager::MissileConfig info;
			// 左
			info = { GetWorldPosition(),Vector3(0,0,0), enemyPtr_->GetTestPtr() };
					
			//missileManager_->BurstTheGravity(info);

			missileManager_->Ashen(info);

			// 連射出来ないように
			isFire_ = true;
		}
		else if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_B && !isFire_) {
			MissileManager::MissileConfig info;
			// 左
			info = { GetWorldPosition(),Vector3(0,0,0), enemyPtr_->GetTestPtr() };

			missileManager_->BurstTheGravity(info);
			// 連射出来ないように
			isFire_ = true;
		}
	}

	if (input_->TriggerKey(DIK_SPACE) && !isFire_) {
		MissileManager::MissileConfig info;
		// 左
		info = { GetWorldPosition(),Vector3(0,0,0), enemyPtr_->GetTestPtr() };

		//missileManager_->BurstTheGravity(info);

		missileManager_->Ashen(info);

		// 連射出来ないように
		isFire_ = true;
	}

	missileManager_->SetPosition(GetWorldPosition());
	if (input_->TriggerKey(DIK_RETURN) && !isFire_) {
		MissileManager::MissileConfig info;
		// 左
		info = { GetWorldPosition(),Vector3(0,0,0), enemyPtr_->GetTestPtr() };

		missileManager_->BurstTheGravity(info);

		// 連射出来ないように
		isFire_ = true;
	}

	if (isFire_) {
		coolTime_++;
		if (coolTime_ > 120) {
			coolTime_ = 0;
			isFire_ = false;
		}
	}

}

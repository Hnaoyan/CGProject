#include "Player.h"
#include "MathCalc.h"
#include "ImGuiManager.h"
#include "GlobalVariables.h"

#include <algorithm>

void Player::Initialize(const std::vector<Model*>& models)
{
	// システム系
	input_ = Input::GetInstance();
	
	// モデル読み込み
	radius_ = 1.0f;
	// ベースの初期化
	models_ = models;
	
	worldTransform_.Initialize();
	worldTransformBody_.Initialize();
	worldTransformL_Arm_.Initialize();
	worldTransformR_Arm_.Initialize();

	// サイズ設定
	collider_.SetterRad(Vector3(radius_, radius_, radius_));
	collider_.SetWorldAddress(&worldTransform_);
	// フラグ初期化
	isDead_ = false;
	isLand_ = false;

	// コライダーの設定
	collider_.SetCollisionAttribute(kCollisionAttributePlayer);
	collider_.SetCollisionMask(0xffffffff - kCollisionAttributePlayer);
	// 関数バインド
	std::function<void(uint32_t, WorldTransform*)> f = std::function<void(uint32_t, WorldTransform*)>(std::bind(&Player::OnCollision, this, std::placeholders::_1, std::placeholders::_2));
	collider_.SetFunction(f);

	worldTransformBody_.parent_ = &worldTransform_;
	worldTransformL_Arm_.parent_ = &worldTransformBody_;
	worldTransformR_Arm_.parent_ = &worldTransformBody_;

	worldTransformL_Arm_.translation_ = { -0.4f, 1.5f, -0.15f };
	worldTransformR_Arm_.translation_ = { 0.4f, 1.5f, -0.15f };

	// 設定
	Setting();
}

void Player::Update()
{
#ifdef _DEBUG
	ImGui::Begin("player");
	ImGui::DragFloat3("pos", &worldTransform_.translation_.x, 0.1f, -20.0f, 20.0f);
	Vector3 pos = { worldTransform_.matWorld_.m[3][0],worldTransform_.matWorld_.m[3][1] ,worldTransform_.matWorld_.m[3][2] };
	ImGui::DragFloat3("matPos", &pos.x, 0.01f, -20.0f, 20.0f);
	ImGui::DragFloat3("rot", &worldTransform_.rotation_.x, 0.1f, -20.0f, 20.0f);
	ImGui::Text("%d", isLand_);
	ImGui::End();
#endif // _DEBUG
	// 死亡判定
	if (worldTransform_.translation_.y < -15.0f) {
		isDead_ = true;
	}
	// リクエストの受付
	if (behaviorRequest_) {
		// 行動変更
		behavior_ = behaviorRequest_.value();
		// 初期化
		switch (behavior_) {
		case Player::Behavior::kRoot:
			//BehaviorRootInitialize();
			break;
		case Player::Behavior::kAttack:
			//BehaviorAttackInitialize();
			break;
		case Player::Behavior::kDash:
			BehaviorDashInitialize();
			break;
		}
		// リクエストをリセット
		behaviorRequest_ = std::nullopt;
	}

	// 移動
	BehaviorRootUpdate();
	// 落下
	Fall();

	// 更新
	BaseCharacter::Update();
	worldTransformBody_.UpdateMatrix();
	worldTransformL_Arm_.UpdateMatrix();
	worldTransformR_Arm_.UpdateMatrix();
}

void Player::Draw(const ViewProjection& viewProjection)
{
	// ベースの描画
	//BaseCharacter::Draw(viewProjection);

	//model_->Draw(worldTransform_, viewProjection);
	models_[BODY]->Draw(worldTransformBody_, viewProjection);
	models_[L_ARM]->Draw(worldTransformL_Arm_, viewProjection);
	models_[R_ARM]->Draw(worldTransformR_Arm_, viewProjection);
}

Vector3 Player::GetWorldPosition()
{
	Vector3 worldPos = { worldTransform_.matWorld_.m[3][0],worldTransform_.matWorld_.m[3][1],worldTransform_.matWorld_.m[3][2] };
	return worldPos;
}

void Player::ProcessMovement()
{
	// 移動
	XINPUT_STATE joyState;
	if (input_->GetInstance()->GetJoystickState(0, joyState)) {
		// 閾値
		const float threshold = 0.7f;
		bool isMoving = false;

		//float angle = 0;

		// 移動量計算
		Vector3 moved = {
			(float)joyState.Gamepad.sThumbLX / SHRT_MAX,0,
			(float)joyState.Gamepad.sThumbLY / SHRT_MAX };
		if (MathCalc::Length(moved) > threshold) {
			isMoving = true;
		}
		// 移動処理
		if (isMoving) {
			// 速さ
			const float speed = 0.3f;
			// 移動量
			Vector3 move = {
				(float)joyState.Gamepad.sThumbLX / SHRT_MAX * speed,0.0f,
				(float)joyState.Gamepad.sThumbLY / SHRT_MAX * speed };
			Vector3 normal = VectorLib::Scaler(MathCalc::Normalize(move), speed);
			velocity_.x = normal.x;
			velocity_.z = normal.z;
			// 向きの処理
			//angle = std::atan2f(move.x, move.z);
			worldTransform_.rotation_.y = std::atan2f(move.x, move.z);
			float length = sqrtf(move.x * move.x + move.z * move.z);
			worldTransform_.rotation_.x = std::atan2f(-move.y, length);
		}
		else {
			//worldTransform_.rotation_.y = 0;
			velocity_.x = 0;
			velocity_.z = 0;
		}
		//worldTransform_.rotation_.y=MathCalc::LerpShortAngle(worldTransform_.rotation_.y,angle,)

		// 座標更新
		worldTransform_.translation_.x += velocity_.x;
		worldTransform_.translation_.z += velocity_.z;
	}

}

void Player::ProcessJump()
{
	if (isLand_) {
		XINPUT_STATE joyState;
		// ジャンプ入力
		if (input_->GetInstance()->GetJoystickState(0, joyState))
		{
			if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER && !isJump_) {
				isJump_ = true;
				worldTransform_.parent_ = nullptr;
				Vector3 worldPosition = GetWorldPosition();
				worldTransform_.translation_ = worldPosition;
				// ジャンプ時の初速度
				isLand_ = false;
				velocity_.y = jumpPower_;
				worldTransform_.translation_.y += velocity_.y;
				// 更新
				worldTransform_.UpdateMatrix();
			}
		}
	}
}

void Player::Fall()
{
	if (!worldTransform_.parent_) {
		velocity_.y += (-0.05f);

		worldTransform_.translation_.y += velocity_.y;
	}
}

void Player::Ground()
{
	velocity_.y = 0;
	isLand_ = true;
	isJump_ = false;
}

void Player::ApplyGlobalVariables()
{
}

void Player::BehaviorRootUpdate()
{
	// ジャンプの処理
	ProcessJump();
	// 移動処理
	ProcessMovement();

	XINPUT_STATE joyState;

	if (input_->GetInstance()->GetJoystickState(0, joyState))
	{
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {
			behaviorRequest_ = Behavior::kDash;
		}
	}

}

void Player::BehaviorDashInitialize()
{
	workDash_.dashParameter_ = 0;
	//worldTransform_.rotation_.y;


}

void Player::BehaviorDashUpdate()
{
}

void Player::BehaviorAttackInitialize()
{
	worldTransformL_Arm_.rotation_.x = 3.0f;
	worldTransformR_Arm_.rotation_.x = 3.0f;
	worldTransformWeapon_.rotation_ = {};
	attackState_ = Attack::kDown;
	workAttack_.stunDuration_ = 0;
}

void Player::BehaviorAttackUpdate()
{
	switch (attackState_) {
	//---攻撃の振りおろし処理---//
	case Player::Attack::kDown:
		worldTransformWeapon_.rotation_.x += 0.02f;
		worldTransformL_Arm_.rotation_.x += 0.02f;
		worldTransformR_Arm_.rotation_.x += 0.02f;
		if (worldTransformWeapon_.rotation_.x > 1.5f) {
			attackState_ = Attack::kStop;
		}
		break;
	//---硬直処理---//
	case Player::Attack::kStop:
		workAttack_.stunDuration_++;
		//硬直終了判定
		if (workAttack_.stunDuration_ == workAttack_.maxStunDuration_) {
			attackState_ = Attack::kDown;
			behaviorRequest_ = Behavior::kRoot;
		}
		break;
	}
}

void Player::OnCollision(uint32_t tag, WorldTransform* targetWorldTransform)
{
	if (tag == kCollisionAttributeEnemy || tag == kCollisionAttributeGoal) {
		DeadToRestart(Vector3(0, 1.0f, 0));
	}
	else if (tag == kCollisionAttributeGround) {
		Vector3 groundWorldPosition = { 
			targetWorldTransform->matWorld_.m[3][0],targetWorldTransform->matWorld_.m[3][1],
			targetWorldTransform->matWorld_.m[3][2] };
		worldTransform_.translation_.y = groundWorldPosition.y + radius_;
		Ground();
		//worldTransform_.parent_ = nullptr;
	}
	else if (tag == kCollisionAttributeMoveGround) {
		if (!worldTransform_.parent_) {
			Vector3 groundWorldPosition = {
				targetWorldTransform->matWorld_.m[3][0],targetWorldTransform->matWorld_.m[3][1],
				targetWorldTransform->matWorld_.m[3][2] };
			Vector3 playerWorldPosition = GetWorldPosition();
			worldTransform_.translation_ = VectorLib::Subtract(playerWorldPosition, groundWorldPosition);
			// 親子関係
			worldTransform_.parent_ = targetWorldTransform;
		}

		if (worldTransform_.translation_.y < 0.f) {
			worldTransform_.translation_.y = radius_;
		}
		Ground();
	}
	else {
		worldTransform_.parent_ = nullptr;
	}
	worldTransform_.UpdateMatrix();
}

void Player::Setting()
{
	//// グローバル変数のjson
	//GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	//// グループ名
	//const char* groupName = "Player";
	//// グループを追加
	//GlobalVariables::GetInstance()->CreateGroup(groupName);


	// リスタート関数
	DeadToRestart(Vector3(0, 1.0f, 0));

}

void Player::DeadToRestart(const Vector3& startPoint)
{
	worldTransform_.parent_ = nullptr;
	worldTransform_.translation_ = startPoint;
	worldTransform_.UpdateMatrix();
	isDead_ = false;
}

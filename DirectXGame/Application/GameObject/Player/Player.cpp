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
	deModel_.reset(Model::Create());

	weapon_ = std::make_unique<Weapon>();
	weapon_->Initialize();
	
	worldTransform_.Initialize();
	worldTransformBody_.Initialize();
	worldTransformL_Arm_.Initialize();
	worldTransformR_Arm_.Initialize();
	worldTransformWeapon_.Initialize();
	worldAttackCollider_.Initialize();

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
	worldTransformWeapon_.parent_ = &worldTransformBody_;
	//worldAttackCollider_.parent_ = &worldTransform_;

	worldTransformBody_.translation_ = { 0,-0.4f,0 };
	worldTransformL_Arm_.translation_ = { -0.4f, 1.5f, -0.15f };
	worldTransformR_Arm_.translation_ = { 0.4f, 1.5f, -0.15f };


#ifdef _DEBUG
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "Player";
	// グループを追加
	GlobalVariables::GetInstance()->CreateGroup(groupName);
	globalVariables->AddItem(groupName, "dashValue", workDash_.dashTime_);

	ApplyGlobalVariables();

#endif // _DEBUG

	// 設定
	Setting();
}

void Player::Update()
{
	// ImGui
	ApplyGlobalVariables();
#ifdef _DEBUG
	ImGui::Begin("player_test");
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
			BehaviorRootInitialize();
			break;
		case Player::Behavior::kAttack:
			BehaviorAttackInitialize();
			break;
		case Player::Behavior::kDash:
			BehaviorDashInitialize();
			break;
		}
		// リクエストをリセット
		behaviorRequest_ = std::nullopt;
	}

	// 状態に応じた処理
	switch (behavior_)
	{
	case Player::Behavior::kRoot:
		BehaviorRootUpdate();
		break;
	case Player::Behavior::kAttack:
		BehaviorAttackUpdate();
		break;
	case Player::Behavior::kDash:
		BehaviorDashUpdate();
		break;
	}

	// 落下
	Fall();

	// 更新
	BaseCharacter::Update();
	worldTransformBody_.UpdateMatrix();
	worldTransformL_Arm_.UpdateMatrix();
	worldTransformR_Arm_.UpdateMatrix();
	worldTransformWeapon_.UpdateMatrix();
	CollisionUpdate();
}

void Player::Draw(const ViewProjection& viewProjection)
{
	models_[BODY]->Draw(worldTransformBody_, viewProjection);
	models_[L_ARM]->Draw(worldTransformL_Arm_, viewProjection);
	models_[R_ARM]->Draw(worldTransformR_Arm_, viewProjection);
	if (behavior_ == Behavior::kAttack) {
		models_[WEAPON]->Draw(worldTransformWeapon_, viewProjection);
	}
	deModel_->Draw(weapon_->GetWorldTransform(), viewProjection);
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
			// 回転行列の合成
			Matrix4x4 rotateMatrix = MatLib::Multiply(
				MatLib::MakeRotateXMatrix(viewProjection_->rotation_.x),
				MatLib::Multiply(
					MatLib::MakeRotateYMatrix(viewProjection_->rotation_.y),
					MatLib::MakeRotateZMatrix(viewProjection_->rotation_.z)));
			move = MatLib::Transform(move, rotateMatrix);
			// 方向取得（仮
			moveDirection_ = move;

			Vector3 normal = VectorLib::Scaler(MathCalc::Normalize(move), speed);
			// 移動速度
			velocity_.x = normal.x;
			velocity_.z = normal.z;
			// 目標角度
			destinationAngleY_ = std::atan2f(move.x, move.z);
			worldTransform_.rotation_.y = std::atan2f(move.x, move.z);
			float length = sqrtf(move.x * move.x + move.z * move.z);
			worldTransform_.rotation_.x = std::atan2f(-move.y, length);
		}
		else {
			velocity_.x = 0;
			velocity_.z = 0;
		}

		//worldTransform_.rotation_.y = MathCalc::LerpShortAngle(worldTransform_.rotation_.y, destinationAngleY_, 0.3f);

		// 座標更新
		worldTransform_.translation_.x += velocity_.x;
		worldTransform_.translation_.z += velocity_.z;
	}

#ifdef _DEBUG
	ImGui::Begin("rotatePlayer");
	ImGui::Text("Lerp_t : %d", rotateLerp_t_);
	ImGui::Text("AngleY : %.2f worldRotationY : %.2f", destinationAngleY_, worldTransform_.rotation_.y);
	ImGui::End();
#endif // _DEBUG

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
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "Player";
	// 移動量
	workDash_.dashTime_ = globalVariables->GetIntValue(groupName, "dashValue");

}

void Player::BehaviorRootInitialize()
{
	//workAttack_ = false;
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
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_B) {
			behaviorRequest_ = Behavior::kDash;
		}
	}

	if (input_->GetInstance()->GetJoystickState(0, joyState) || input_->TriggerKey(DIK_SPACE)) {
		if (joyState.Gamepad.bRightTrigger) {
			behaviorRequest_ = Behavior::kAttack;
		}
	}
}

void Player::BehaviorDashInitialize()
{
	workDash_.dashParameter_ = 0;
}

void Player::BehaviorDashUpdate()
{
	// 移動処理
	// 方向取得
	float x = std::cosf(worldTransform_.rotation_.x) * std::sinf(worldTransform_.rotation_.y);
	float y = -std::sinf(worldTransform_.rotation_.x);
	float z = std::cosf(worldTransform_.rotation_.x) * std::cosf(worldTransform_.rotation_.y);

	Vector3 moveDirect = { x ,y ,z };
	moveDirect = VectorLib::Scaler(MathCalc::Normalize(moveDirect), 0.2f);

	velocity_ = VectorLib::Add(velocity_, moveDirect);

	worldTransform_.translation_.x += velocity_.x;
	worldTransform_.translation_.z += velocity_.z;

	// ダッシュ時間<Frame>
	const uint32_t behaviorDashTime = workDash_.dashTime_;

	// 既定の時間経過で元にもどる
	if (++workDash_.dashParameter_ >= behaviorDashTime) {
		behaviorRequest_ = Behavior::kRoot;
	}
}

void Player::BehaviorAttackInitialize()
{
	worldTransformL_Arm_.rotation_.x = 3.0f;
	worldTransformR_Arm_.rotation_.x = 3.0f;
	worldTransformWeapon_.rotation_ = {};
	attackState_ = Attack::kDown;
	workAttack_.attackParameter_ = 0;
}

void Player::BehaviorAttackUpdate()
{
	switch (attackState_) {
	//---攻撃の振りおろし処理---//
	case Player::Attack::kDown:
		worldTransformWeapon_.rotation_.x += 0.05f;
		worldTransformL_Arm_.rotation_.x += 0.05f;
		worldTransformR_Arm_.rotation_.x += 0.05f;
		if (worldTransformWeapon_.rotation_.x > 1.5f) {
			attackState_ = Attack::kStop;
		}
		break;
	//---硬直処理---//
	case Player::Attack::kStop:
		workAttack_.attackParameter_++;
		//硬直終了判定
		const float kMaxDuration = 30;
		if (workAttack_.attackParameter_ == kMaxDuration) {
			worldTransformL_Arm_.rotation_.x = 0;
			worldTransformR_Arm_.rotation_.x = 0;
			attackState_ = Attack::kDown;
			behaviorRequest_ = Behavior::kRoot;
		}
		break;
	}

	//XINPUT_STATE joyStatePre;
	XINPUT_STATE joyState;

	// コンボ上限に達していない
	if (workAttack_.comboIndex_ < ComboNum) {

		//
		if (input_->GetInstance()->GetJoystickState(0, joyState)) {
			// 攻撃トリガー
			if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A) {
				// コンボ有効
				workAttack_.comboNext_ = true;
			}
		}

	}

	// 既定の時間経過で通常行動に戻る
	if (++workAttack_.attackParameter_ >= 1) {
		// コンボ継続なら次のコンボに進む
		if (workAttack_.comboNext_) {
			// フラグリセット
			workAttack_.comboNext_ = false;

		}
		// コンボ継続でないなら攻撃を終了してルートビヘイビアに戻る
		else {
			behaviorRequest_ = Behavior::kRoot;
		}
	}

	// コンボ段階によってモーションを分岐
	switch (workAttack_.comboIndex_)
	{
	case 0:
		break;
	case 1:
		break;
	case 2:
		break;
	}


}

void Player::CollisionUpdate()
{
	// 方向取得
	float x = std::cosf(worldTransform_.rotation_.x) * std::sinf(worldTransform_.rotation_.y);
	float y = -std::sinf(worldTransform_.rotation_.x);
	float z = std::cosf(worldTransform_.rotation_.x) * std::cosf(worldTransform_.rotation_.y);

	float distance = 3.0f;

	Vector3 worldPosition = GetWorldPosition();

	Vector3 collisionPoint = {
		worldPosition.x + x * distance,
		worldPosition.y + y * distance,
		worldPosition.z + z * distance,
	};

	weapon_->SetWorldPosition(collisionPoint);
	weapon_->Update();
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

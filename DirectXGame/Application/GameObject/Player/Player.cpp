#include "Player.h"
#include "MathCalc.h"
#include "ImGuiManager.h"

#include <algorithm>

void Player::Initialize(Model* model)
{
	// システム系
	input_ = Input::GetInstance();

	// モデル読み込み
	radius_ = 1.0f;
	BaseCharacter::Initialize(model);
	collider_.SetCollisionAttribute(kCollisionAttributePlayer);
	collider_.SetCollisionMask(0xffffffff - kCollisionAttributePlayer);
	isDead_ = false;
	isLand_ = false;
	std::function<void(uint32_t, WorldTransform*)> f = std::function<void(uint32_t, WorldTransform*)>(std::bind(&Player::OnCollision, this, std::placeholders::_1, std::placeholders::_2));
	collider_.SetFunction(f);
	DeadToRestart(Vector3(0, 1.0f, 0));
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

	// ジャンプ
	if (isLand_) {
		Jump();
	}
	// 更新処理
	BaseCharacter::Update();
	// 移動入力などの処理
	Move();
	// 落下
	Fall();

	// 死亡判定
	if (worldTransform_.translation_.y < -15.0f) {
		isDead_ = true;
	}
}

void Player::Draw(const ViewProjection& viewProjection)
{
	// ベースの描画
	BaseCharacter::Draw(viewProjection);

}

void Player::OnCollisionObject()
{
}

Vector3 Player::GetWorldPosition()
{
	Vector3 worldPos = { worldTransform_.matWorld_.m[3][0],worldTransform_.matWorld_.m[3][1],worldTransform_.matWorld_.m[3][2] };
	return worldPos;
}

void Player::Move()
{
	XINPUT_STATE joyState;
	if (input_->GetInstance()->GetJoystickState(0, joyState)) {
		const float threshold = 0.7f;
		bool isMoving = false;

		Vector3 moved = {
			(float)joyState.Gamepad.sThumbLX / SHRT_MAX,0,
			(float)joyState.Gamepad.sThumbLY / SHRT_MAX };
		if (MathCalc::Length(moved) > threshold) {
			isMoving = true;
		}
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
			worldTransform_.rotation_.y = std::atan2f(move.x, move.z);
			float length = sqrtf(move.x * move.x + move.z * move.z);
			worldTransform_.rotation_.x = std::atan2f(-move.y, length);
			// 移動処理
		}
		else {
			worldTransform_.rotation_.y = 0;
			velocity_.x = 0;
			velocity_.z = 0;
		}
		worldTransform_.translation_.x += velocity_.x;
		worldTransform_.translation_.z += velocity_.z;
	}
}

void Player::Jump()
{
	XINPUT_STATE joyState;
	// ジャンプ入力
	if (input_->GetInstance()->GetJoystickState(0, joyState))
	{
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER && !isJump_) {
			isJump_ = true;
			worldTransform_.parent_ = nullptr;
			Vector3 worldPosition = GetWorldPosition();
			worldTransform_.translation_ = worldPosition;
			worldTransform_.UpdateMatrix();
		}
	}

	if (isJump_) {
		isLand_ = false;
		velocity_.y = jumpPower_;
		worldTransform_.translation_.y += velocity_.y;
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

void Player::DeadToRestart(const Vector3& startPoint)
{
	worldTransform_.parent_ = nullptr;
	worldTransform_.translation_ = startPoint;
	worldTransform_.UpdateMatrix();
	isDead_ = false;
}

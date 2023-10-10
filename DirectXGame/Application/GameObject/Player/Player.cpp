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
	isLand_ = true;
	std::function<void(uint32_t, WorldTransform*)> f = std::function<void(uint32_t, WorldTransform*)>(std::bind(&Player::OnCollision, this, std::placeholders::_1, std::placeholders::_2));
	collider_.SetFunction(f);
}

void Player::Update()
{
	ImGui::Begin("player");
	ImGui::DragFloat3("pos", &worldTransform_.translation_.x, 0.1f, -20.0f, 20.0f);
	ImGui::DragFloat3("rot", &worldTransform_.rotation_.x, 0.1f, -20.0f, 20.0f);
	ImGui::Text("%d", isDead_);
	ImGui::End();
	velocity_ = {};
	isDead_ = false;

	// 移動入力などの処理
	Move();

	if (isLand_) {
		Jump();
	}
	else {
		Fall();
	}
	if (worldTransform_.translation_.y < 0) {
		worldTransform_.translation_.y = 0;
		isLand_ = true;
		isJump_ = false;
	}

	// 基底クラスの処理
	BaseCharacter::Update();

}

void Player::Draw(const ViewProjection& viewProjection)
{
	// ベースの描画
	BaseCharacter::Draw(viewProjection);

}

void Player::OnCollisionObject()
{
	worldTransform_.translation_.y = 0;
	velocity_.y = 0;
	isJump_ = false;
	isLand_ = true;
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
			(float)joyState.Gamepad.sThumbLY / SHRT_MAX};
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

			velocity_ = VectorLib::Scaler(MathCalc::Normalize(move), speed);
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
		}
	}

	if (isJump_) {
		isLand_ = false;
		velocity_.y = 3.0f;
		worldTransform_.translation_.y += velocity_.y;
	}

}

void Player::Fall()
{
	velocity_.y += (-0.1f);

	worldTransform_.translation_.y += velocity_.y;
}

void Player::OnCollision(uint32_t tag, WorldTransform* world)
{
	if (tag == kCollisionAttributeEnemy || tag == kCollisionAttributeGoal) {
		DeadToRestart(Vector3(0, 2.0f, 0));
	}
	if (tag == kCollisionAttributeGround) {
		Vector3 pos = { world->matWorld_.m[3][0],world->matWorld_.m[3][1],world->matWorld_.m[3][2] };
		worldTransform_.translation_.y += pos.y + radius_;
		velocity_.y = 0;
		isLand_ = true;
		isJump_ = false;
	}
}

void Player::DeadToRestart(const Vector3& startPoint)
{
	worldTransform_.translation_ = startPoint;
	isDead_ = false;
}

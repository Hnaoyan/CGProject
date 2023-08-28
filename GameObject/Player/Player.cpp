#include "Player.h"
#include "Vector3Math.h"
#include "MathCalc.h"
#include "Input.h"
#include "MatrixMath.h"
#include "ImGuiManager.h"
#include "CollisionConfig.h"

Player::~Player() {
	for (PlayerBullet* bullet : bullets_) {
		delete bullet;
	}
}

void Player::BehaviorJumpUpdate() 
{ 
	fallTime_ += 1;
	float fallSpeed = 10.0f - 9.8f * fallTime_ / 60;
	worldTransform_.translation_.y += fallSpeed;

}

void Player::BehaviorJumpInitialize() 
{
	jumpPower_ = 10.0f;
	fallTime_ = 0;
	worldTransform_.translation_.y += jumpPower_;
}

void Player::Initialize(Model* model) 
{ 
	// 基底クラスの初期化
	BaseCharacter::Initialize(model);
	worldTransform_.translation_.y = 1.0f;
	this->SetCollisionAttribute(kCollisionAttributePlayer);
	this->SetCollisionMask(kCollisionAttributeEnemy);
}

void Player::Update() 
{ 
	bullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

	//ImGui::Begin("player");
	//ImGui::DragFloat3("pos", &worldTransform_.translation_.x, 0.01f, -20.0f, 20.0f);
	//ImGui::DragFloat3("rotate", &worldTransform_.rotation_.x, 0.01f, -20.0f, 20.0f);
	//ImGui::DragFloat("jumpPower", &jumpPower_, 0.01f, -20.0f, 20.0f);
	//ImGui::Text("BulletCount : %d", this->bulletCount);
	//ImGui::Text("isReload : %d", this->isReload);
	//ImGui::Text("ReloadTime : %d", this->reloadCount);
	//ImGui::End();


	XINPUT_STATE joyState;
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		// 
		float speed = 0.5f;

		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER && !dash.isDash) {
			speed = 10.0f;
			dash.isDash = true;
		}

		//
		Vector3 move = {
		    (float)joyState.Gamepad.sThumbLX / SHRT_MAX * speed, 0.0f,
		    (float)joyState.Gamepad.sThumbLY / SHRT_MAX * speed};

		// 
		move = Vector3Math::Scaler(MathCalc::Normalize(move), speed);

		// 移動ベクトルを回転方向に変換
		Matrix4x4 rotate = MatrixMath::Multiply(
		    MatrixMath::MakeRotateXMatrix(worldTransform_.rotation_.x),
		    MatrixMath::Multiply(
		        MatrixMath::MakeRotateYMatrix(worldTransform_.rotation_.y),
		        MatrixMath::MakeRotateZMatrix(worldTransform_.rotation_.z)));

		move = MatrixMath::Transform(move, rotate);

		// 移動処理
		worldTransform_.translation_ = Vector3Math::Add(worldTransform_.translation_, move);

	}

	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		
		float rotateSpeed = 0.1f;
		worldTransform_.rotation_.y += (float)joyState.Gamepad.sThumbRX / SHRT_MAX * rotateSpeed;


	}

	if (dash.isDash) {
		dash.coolDownCount += 1;
		if (dash.coolDownCount == 10) {
			dash.isDash = false;
			dash.coolDownCount = 0;
		}
	}

	Jump();

	BaseCharacter::Update();

	Attack();
	for (PlayerBullet* bullet : bullets_) {
		bullet->Update();
	}

}

void Player::Draw(const ViewProjection& viewProjection) 
{
	BaseCharacter::Draw(viewProjection); 

	for (PlayerBullet* bullet : bullets_) {
		bullet->Draw(viewProjection);
	}

}

void Player::Attack() { 
	XINPUT_STATE joyState;
	// ゲームパッド未接続なら何もせずに抜ける
	if (!Input::GetInstance()->GetJoystickState(0, joyState)) {
		return;
	}
	// Rトリガーを押していたら
	if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER && !isShot && !isReload) {
		// 弾速
		const float kBulletSpeed = 5.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		this->bulletCount += 1;

		velocity = MatrixMath::TransformNormal(velocity, worldTransform_.matWorld_);

		// 弾生成
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(bulletModel_,
			worldTransform_.translation_, velocity);

		newBullet->SetRotate(this->worldTransform_.rotation_);

		//
		bullets_.push_back(newBullet);
		isShot = true;
	}

	if (bulletCount == 25) {
		isReload = true;
		bulletCount = 0;
	}

	if (isReload) {
		const int ReloadClearCount = 200;
		reloadCount++;
		if (reloadCount == ReloadClearCount) {
			isReload = false;
			reloadCount = 0;
		}
	}

	if (isShot) {
		++coolTime;
		if(coolTime == Interval) {
			coolTime=0;
			isShot = false;
		}
	}

}

void Player::Jump() 
{
	XINPUT_STATE joyState;
	// ゲームパッド未接続なら何もせずに抜ける
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_B && !isJump_) {
			isJump_ = true;
			fallTime_ = 0;
			jumpVelocity_.y = jumpPower_;
		}
	}

	if (isJump_) {
		this->fallTime_ += 1;
		worldTransform_.translation_.y += jumpVelocity_.y + (-9.8f * (fallTime_) / 60.0f);
	}

	if (worldTransform_.translation_.y < 1.2f) {
		isJump_ = false;
		worldTransform_.translation_.y = 1.0f;
	}
}

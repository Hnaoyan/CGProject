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

}

void Player::BehaviorJumpInitialize() 
{

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
	XINPUT_STATE joyState;
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		// 
		float speed = 0.2f;

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

	if (input_->GetInstance()->PressKey(DIK_W)) {
		worldTransform_.translation_.z += 0.2f;
	}
	if (input_->GetInstance()->PressKey(DIK_S)) {
		worldTransform_.translation_.z -= 0.2f;
	}

	/// カメラの更新
	CameraUpdate();

	/// ジャンプ用の処理
	Jump();

	BulletUpdate();

	/// ベースの更新
	BaseCharacter::Update();

	/// 攻撃の更新
	Attack();
}

void Player::Draw(const ViewProjection& viewProjection) 
{
	/// ベースの描画
	BaseCharacter::Draw(viewProjection); 

	for (PlayerBullet* bullet : bullets_) {
		bullet->Draw(viewProjection);
	}

}

void Player::CameraUpdate()
{
	XINPUT_STATE joyState;

	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		const float threshold = 0.7f;
		bool isRotation = false;

		// スティックの操作
		float rotateZ = (float)joyState.Gamepad.sThumbRX / SHRT_MAX;
		float length = sqrt(powf(rotateZ, 2));
		if (length > threshold) {
			isRotation = true;
		}
		if (isRotation) {
			float rotateSpeed = 0.005f;
			worldTransform_.rotation_.y += (float)joyState.Gamepad.sThumbRX / SHRT_MAX * rotateSpeed;
		}
	}

}

void Player::BulletUpdate()
{
	bullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

	for (PlayerBullet* bullet : bullets_) {
		bullet->Update();
	}

}

void Player::Attack() {

}

void Player::Jump() 
{

}

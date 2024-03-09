#include "Enemy.h"
#include "TextureManager.h"
#include "NLib.h"
#include "imgui.h"

void Enemy::Initialize(Model* model,Model* propModel)
{

	model_ = model;
	propModel_ = propModel;
	texture_ = TextureManager::Load("white1x1.png");
	//velocity_ = { 0,1,0.5f };
	TransformInitialize();

	radius_ = 1.0f;
	collider_.SetterRad(Vector3(radius_, radius_, radius_));
	collider_.SetCollisionAttribute(kCollisionAttributeEnemy);
	collider_.SetWorldAddress(&worldTransform_);
	std::function<void(uint32_t, WorldTransform*)> f = std::function<void(uint32_t, WorldTransform*)>(std::bind(&Enemy::OnCollision, this, std::placeholders::_1, std::placeholders::_2));
	collider_.SetFunction(f);
	
	isOnHit_ = false;
	moveType_ = kOne;
}

void Enemy::TransformInitialize()
{
	worldTransform_.Initialize();
	worldTransform_.translation_.z = 150.0f;

	bodyTransform_.Initialize();
	propTransform_.Initialize();

	bodyTransform_.parent_ = &worldTransform_;
	propTransform_.parent_ = &bodyTransform_;

	propTransform_.translation_.y = 2.0f;

	//worldTransform_.rotation_.y = 3.14f;
}

void Enemy::Update()
{
	moveCount_++;
	//stickVector_ = {};
	//const int kChengeCount = 450;

	//float moveSpeed = 5.0f;

	//if (GetWorldPosition().y > 28.0f) {
	//	velocity_.y = -1.0f;
	//}
	//if (GetWorldPosition().y <= -30.0f) {
	//	velocity_.y = 1.0f;
	//}

	propTransform_.rotation_.y += 0.1f;

	if (Input::GetInstance()->PressKey(DIK_LEFT)) {
		velocity_.x = -10.0f;
	}
	else if (Input::GetInstance()->PressKey(DIK_RIGHT)) {
		velocity_.x = 10.0f;
	}
	else {
		velocity_.x = 0;
	}

	if (Input::GetInstance()->PressKey(DIK_UP)) {
		velocity_.y = 10.0f;
	}
	else if (Input::GetInstance()->PressKey(DIK_DOWN)) {
		velocity_.y = -10.0f;
	}
	else {
		velocity_.y = 0;
	}

	if (Input::GetInstance()->PressKey(DIK_Y)) {
		velocity_.z = 10.0f;
	}
	else if (Input::GetInstance()->PressKey(DIK_H)) {
		velocity_.z = -10.0f;
	}
	else {

	}
	velocity_.z = 10.0f;

	//switch (moveType_)
	//{
	//case kOne:
	//	if (moveCount_ > kChengeCount) {
	//		moveCount_ = 0;
	//		moveType_ = kTwo;
	//		velocity_ = { 0.2f,-0.6f,0.1f };
	//		velocity_ *= moveSpeed;
	//	}
	//	break;
	//case kTwo:
	//	if (moveCount_ > kChengeCount) {
	//		moveCount_ = 0;
	//		moveType_ = kThree;
	//		velocity_ = { -0.3f,-0.1f,0.2f };
	//		velocity_ *= moveSpeed;
	//	}
	//	break;
	//case kThree:
	//	if (moveCount_ > kChengeCount) {
	//		moveCount_ = 0;
	//		moveType_ = kFour;
	//		velocity_ = { +0.3f,0.5f,-0.4f };
	//		velocity_ *= moveSpeed;
	//	}
	//	break;
	//case kFour:
	//	if (moveCount_ > kChengeCount) {
	//		moveCount_ = 0;
	//		moveType_ = kOne;
	//		velocity_ = { 0.0f,0.5f,0.1f };
	//		velocity_ *= moveSpeed;
	//	}
	//	break;
	//}
	XINPUT_STATE joyState;
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		//moveVector.x += (float)joyState.Gamepad.sThumbLX / SHRT_MAX * SpeedDelta(5.0f);
		stickVector_.x += (float)joyState.Gamepad.sThumbRX / SHRT_MAX * SpeedDelta(5.0f);
		stickVector_.y += (float)joyState.Gamepad.sThumbRY / SHRT_MAX * SpeedDelta(5.0f);
		//worldTransform_.rotation_.z = -moveDirect_.x;
	}

	if (isDead_) {
		invisibleTimer_++;
		int kResetTime = 0;
		if (invisibleTimer_ > kResetTime) {
			isDead_ = false;
			invisibleTimer_ = 0;
		}
	}

//	Vector3 moveVector{};
//
//#ifdef _DEBUG
//	float speedValue = 10.0f;
//
//	XINPUT_STATE joyState;
//	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
//		moveVector.x += (float)joyState.Gamepad.sThumbRX / SHRT_MAX * speedValue;
//		moveVector.y += (float)joyState.Gamepad.sThumbRY / SHRT_MAX * speedValue;
//		moveVector.z += (float)joyState.Gamepad.sThumbLY / SHRT_MAX * speedValue;
//	}
//
	ImGuiWidget();
//
//#endif // _DEBUG

	worldTransform_.translation_ += (velocity_) * NLib::GetDeltaTime(60.0f);

	worldTransform_.translation_ += (stickVector_) * NLib::GetDeltaTime(60.0f);

	worldTransform_.translation_.z += 3.0f * NLib::GetDeltaTime(60.0f);

	worldTransform_.UpdateMatrix();
	
	Animation();

	bodyTransform_.UpdateMatrix();
	propTransform_.UpdateMatrix();
	collider_.SetPosition(worldTransform_.GetWorld());
}

void Enemy::Draw(const ViewProjection& viewProjection)
{
	model_->Draw(bodyTransform_, viewProjection);
	propModel_->Draw(propTransform_, viewProjection);
}

void Enemy::Animation()
{

	// 1フレームでのパラメータ加算値
	const float step = 2.0f * float(std::numbers::pi) / period;

	// パラメータを1ステップ分加算
	floatingParameter_ += step;
	// 2πを超えたら0に戻す
	floatingParameter_ = std::fmod(floatingParameter_, 2.0f * float(std::numbers::pi));

	// 浮遊を座標に反映
	bodyTransform_.translation_.y = std::sin(floatingParameter_) * floatingWidth;
	

}

void Enemy::ImGuiWidget()
{
	ImGui::Begin("enemy");

	ImGui::DragFloat3("pos", &worldTransform_.translation_.x);

	ImGui::End();

}

void Enemy::OnCollision(uint32_t tag, WorldTransform* world)
{
	world, tag;
	if (!isDead_) {
		isOnHit_ = true;
	}
	isDead_ = true;
}

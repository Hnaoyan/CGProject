#include "Enemy.h"
#include "TextureManager.h"
#include "NLib.h"
#include "imgui.h"

void Enemy::Initialize(Model* model)
{

	model_ = model;
	worldTransform_.Initialize();
	texture_ = TextureManager::Load("white1x1.png");
	//velocity_ = { 0,1,0.5f };
	worldTransform_.translation_.z = 50.0f;
	radius_ = 1.0f;
	collider_.SetterRad(Vector3(radius_, radius_, radius_));
	collider_.SetCollisionAttribute(kCollisionAttributeEnemy);
	collider_.SetWorldAddress(&worldTransform_);
	std::function<void(uint32_t, WorldTransform*)> f = std::function<void(uint32_t, WorldTransform*)>(std::bind(&Enemy::OnCollision, this, std::placeholders::_1, std::placeholders::_2));
	collider_.SetFunction(f);
	
	isOnHit_ = false;
	moveType_ = kOne;
}

void Enemy::Update()
{
	moveCount_++;

	//const int kChengeCount = 450;

	//float moveSpeed = 5.0f;

	//if (GetWorldPosition().y > 28.0f) {
	//	velocity_.y = -1.0f;
	//}
	//if (GetWorldPosition().y <= -30.0f) {
	//	velocity_.y = 1.0f;
	//}

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
		velocity_.z = 0;
	}

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

	if (isDead_) {
		invisibleTimer_++;
		int kResetTime = 0;
		if (invisibleTimer_ > kResetTime) {
			isDead_ = false;
			invisibleTimer_ = 0;
		}
	}

	Vector3 moveVector{};

#ifdef _DEBUG
	float speedValue = 10.0f;

	XINPUT_STATE joyState;
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		moveVector.x += (float)joyState.Gamepad.sThumbRX / SHRT_MAX * speedValue;
		moveVector.y += (float)joyState.Gamepad.sThumbRY / SHRT_MAX * speedValue;
		moveVector.z += (float)joyState.Gamepad.sThumbLY / SHRT_MAX * speedValue;
	}

	ImGuiWidget();

#endif // _DEBUG

	worldTransform_.translation_ += (velocity_ + moveVector) * NLib::GetDeltaTime(60.0f);

	worldTransform_.UpdateMatrix();
	collider_.SetPosition(worldTransform_.GetWorld());
}

void Enemy::Draw(const ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, texture_);
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

#include "Enemy.h"
#include "TextureManager.h"
#include "NLib.h"
#include "imgui.h"

void Enemy::Initialize(Model* model)
{

	model_ = model;
	worldTransform_.Initialize();
	texture_ = TextureManager::Load("white1x1.png");
	velocity_ = { 0,1,1 };

	radius_ = 1.0f;
	collider_.SetterRad(Vector3(radius_, radius_, radius_));
	collider_.SetCollisionAttribute(kCollisionAttributeEnemy);
	collider_.SetWorldAddress(&worldTransform_);
	std::function<void(uint32_t, WorldTransform*)> f = std::function<void(uint32_t, WorldTransform*)>(std::bind(&Enemy::OnCollision, this, std::placeholders::_1, std::placeholders::_2));
	collider_.SetFunction(f);

}

void Enemy::Update()
{

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
	//velocity_ = {};

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
	isDead_ = true;
}

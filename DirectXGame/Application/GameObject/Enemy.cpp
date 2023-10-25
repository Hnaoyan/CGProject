#include "Enemy.h"
#include <numbers>
#include "ImGuiManager.h"

void Enemy::Initialize(Model* model)
{
	model;
	radius_ = 1.0f;
	worldTransform_.Initialize();
	worldTransform_.translation_.x = 0;
	worldTransform_.translation_.z = 40.0f;
	collider_.SetterRad(Vector3(radius_, radius_, radius_));
	collider_.SetCollisionAttribute(kCollisionAttributeEnemy);
	collider_.SetCollisionMask(0xffffffff - kCollisionAttributeEnemy);
	collider_.SetWorldAddress(&worldTransform_);
	std::function<void(uint32_t, WorldTransform*)> f = std::function<void(uint32_t, WorldTransform*)>(std::bind(&Enemy::OnCollision, this, std::placeholders::_1, std::placeholders::_2));
	collider_.SetFunction(f);

	MoveInitialize();

}

void Enemy::Update()
{
	UpdateFloating();

	Move();

	BaseCharacter::Update();	
	worldBody_.UpdateMatrix();
	worldL_arm_.UpdateMatrix();
	worldR_arm_.UpdateMatrix();
}

void Enemy::Draw(const ViewProjection& viewProjection)
{
	//BaseCharacter::Draw(viewProjection);
	models_[BODY]->Draw(worldBody_, viewProjection);
	models_[L_ARM]->Draw(worldL_arm_, viewProjection);
	models_[R_ARM]->Draw(worldR_arm_, viewProjection);
}

void Enemy::Move()
{
	
	ImGui::Begin("state");
	ImGui::Text("%d", isLeft_);
	ImGui::DragFloat3("pos", &worldTransform_.translation_.x, 0.01f, -10.0f, 10.0f);
	ImGui::End();
	
	float returnPositionX = 3.0f;

	if (isLeft_) 
	{
		if (worldTransform_.translation_.x < -returnPositionX) {
			isLeft_ = false;
			velocity_.x *= -1.0f;
		}
	}
	else
	{
		if (worldTransform_.translation_.x > returnPositionX) {
			isLeft_ = true;
			velocity_.x *= -1.0f;
		}
	}

	worldTransform_.translation_.x += velocity_.x;

	worldTransform_.rotation_.y = std::atan2f(velocity_.x, velocity_.z);
	float length = sqrtf(velocity_.x * velocity_.x + velocity_.z * velocity_.z);
	worldTransform_.rotation_.x = std::atan2f(-velocity_.y, length);

}

void Enemy::SetModel(const std::vector<Model*>& models)
{
	models_ = models;

	worldBody_.Initialize();
	worldL_arm_.Initialize();
	worldR_arm_.Initialize();

	worldBody_.parent_ = &worldTransform_;
	worldL_arm_.parent_ = &worldBody_;
	worldR_arm_.parent_ = &worldBody_;

	worldL_arm_.translation_ = { -0.5f,1.5f,-0.15f };
	worldR_arm_.translation_ = { 0.5f,1.5f,-0.15f };

	InitializeFloating();
}

void Enemy::OnCollision(uint32_t tag, WorldTransform* targetWorldTransform)
{
	tag;
	targetWorldTransform;
}

Vector3 Enemy::GetWorldPosition()
{
	return worldTransform_.translation_;
}

void Enemy::UpdateFloating()
{
	// 1フレームでのパラメータ加算値
	const float step = 2.0f * float(std::numbers::pi) / period;

	// パラメータを1ステップ分加算
	floatingParameter_ += step;
	// 2πを超えたら0に戻す
	floatingParameter_ = std::fmod(floatingParameter_, 2.0f * float(std::numbers::pi));

	// 浮遊を座標に反映
	worldBody_.translation_.y = std::sin(floatingParameter_) * floatingWidth;

	worldL_arm_.rotation_.x = std::sin(floatingParameter_) * floatingWidth;
	worldR_arm_.rotation_.x = std::sin(floatingParameter_) * floatingWidth;
}

void Enemy::InitializeFloating()
{
	floatingParameter_ = 0.0f;
}

void Enemy::MoveInitialize()
{
	isLeft_ = false;
	velocity_ = { 0.1f,0,0 };
}

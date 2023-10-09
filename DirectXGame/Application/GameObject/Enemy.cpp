#include "Enemy.h"
#include <numbers>

void Enemy::Initialize(Model* model)
{
	model;
	radius_ = 1.0f;
	//BaseCharacter::Initialize(model);
	collider_.SetterRad(radius_);
	worldTransform_.Initialize();
	worldTransform_.translation_.z = 20.0f;
	collider_.SetCollisionAttribute(kCollisionAttributeEnemy);
	collider_.SetCollisionMask(0xffffffff - kCollisionAttributeEnemy);
}

void Enemy::Update()
{
	UpdateFloating();

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

void Enemy::OnCollision()
{
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

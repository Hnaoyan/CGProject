#include "BaseCharacter.h"
#include "MathCalc.h"

void BaseCharacter::Initialize(Model* model) {

	model_ = model;
	worldTransform_.Initialize();
	// コライダーの設定
	ColliderSetting();
}

void BaseCharacter::Update() 
{
	collider_.SetPosition(worldTransform_.translation_);

	worldTransform_.UpdateMatrix();
}

void BaseCharacter::Draw(const ViewProjection& viewProjection) 
{
	model_->Draw(worldTransform_, viewProjection);

}

void BaseCharacter::ColliderSetting()
{
	// サイズ設定
	collider_.SetterRad(radius_);

}

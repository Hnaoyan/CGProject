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
	Vector3 pos = { worldTransform_.matWorld_.m[3][0],worldTransform_.matWorld_.m[3][1],worldTransform_.matWorld_.m[3][2] };
	collider_.SetPosition(pos);
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
	collider_.SetWorldAddress(&worldTransform_);
}

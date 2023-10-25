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
	worldTransform_.UpdateMatrix();
	Vector3 pos = { worldTransform_.matWorld_.m[3][0],worldTransform_.matWorld_.m[3][1],worldTransform_.matWorld_.m[3][2] };
	collider_.SetPosition(pos);
}

void BaseCharacter::Draw(const ViewProjection& viewProjection) 
{
	model_->Draw(worldTransform_, viewProjection);

}

void BaseCharacter::ColliderSetting()
{
	// サイズ設定
	collider_.SetterRad(Vector3(radius_, radius_, radius_));
	collider_.SetWorldAddress(&worldTransform_);
}

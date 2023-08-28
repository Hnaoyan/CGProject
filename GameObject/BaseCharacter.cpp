#include "BaseCharacter.h"
#include "MatrixMath.h"

void BaseCharacter::Initialize(Model* model) {

	model_ = model;
	worldTransform_.Initialize();
}

void BaseCharacter::Update() 
{ 
	worldTransform_.UpdateMatrix();

}

void BaseCharacter::Draw(const ViewProjection& viewProjection) 
{
	model_->Draw(worldTransform_, viewProjection);

}

void BaseCharacter::OnCollision() {}

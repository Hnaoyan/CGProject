#include "BaseBullet.h"
#include "MathCalc.h"
#include <cassert>

void BaseBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity) 
{
	// Nullチェック
	assert(model);

	this->model_ = model;

	// 初期化
	worldTransform_.Initialize();

	this->worldTransform_.translation_ = position;

	this->velocity_ = velocity;

}

void BaseBullet::Update() 
{
	// 座標移動
	worldTransform_.translation_ = VectorLib::Add(worldTransform_.translation_, velocity_);

	// 行列の更新
	this->worldTransform_.UpdateMatrix();

}

void BaseBullet::Draw(const ViewProjection& viewProjection) 
{
	// 描画
	model_->Draw(worldTransform_, viewProjection);
}

Vector3 BaseBullet::GetWorldPosition() 
{ 
	Vector3 worldPos{};

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

void BaseBullet::OnCollision() 
{
	isDead_ = true;
}

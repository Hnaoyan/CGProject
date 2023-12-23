#include "IMissile.h"
#include "NLib.h"
#include "imgui.h"

#include <algorithm>

void IMissile::Initialize(Model* model)
{
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.scale_ = { 0.5f,0.5f,0.5f };
}

void IMissile::Update()
{
#ifdef _DEBUG
	ImGui::Begin("Missile");
	ImGui::Text("frame : %.3f", frame_);
	ImGui::Text("P : %.3f", p_);
	ImGui::End();
#endif // _DEBUG
	//float delta = (1.0f / 60.0f) * kBulletSpeed_;
	//velocity_ += VectorLib::Scaler(direction_, delta);
	//Vector3 directToEnemy = target_->GetWorldPosition() - GetWorldPosition();
	guidedTime_++;
	if (guidedTime_ > 15) {
		frame_ += 1.0f / 600.0f;
		frame_ = std::clamp(frame_, 0.0f, 1.0f);
		guidedTime_ = 0;
		//Vector3 directToEnemy = GetWorldPosition() - target_->GetWorldPosition();
		Vector3 directToEnemy = target_->GetWorldPosition() - GetWorldPosition();
		direction_ = NLib::InductionNewVector(direction_, directToEnemy, frame_, 0.01f);
		p_ = NLib::InductionPerform(frame_, 0.01f);
	}
	velocity_ += VectorLib::Scaler(direction_, 0.2f);
	worldTransform_.translation_ += velocity_;
	worldTransform_.UpdateMatrix();
}

void IMissile::Draw(ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection);
}

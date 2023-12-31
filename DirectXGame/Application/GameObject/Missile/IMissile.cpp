#include "IMissile.h"
#include "NLib.h"
#include "imgui.h"
#include "MissileManager.h"

#include <algorithm>

void IMissile::Initialize(Model* model, const Vector3& position)
{
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.scale_ = { 0.5f,0.5f,0.5f };
}

void IMissile::Update()
{
#ifdef _DEBUG
	ImGui::Begin("Missile");
	ImGui::Text("frame : %.3f", frame_);
	ImGui::Text("P : %.3f", p_);
	ImGui::DragFloat3("direct", &velocity_.x);
	ImGui::DragFloat("lerpRad", &lerpRad_, 0.1f, 0, 30.0f);
	ImGui::DragFloat("damping", &damping_, 0.01f, 0, 1.0f);
	ImGui::DragInt("coolTime", &coolTime_, 1, 0, 100);
	ImGui::End();
#endif // _DEBUG

	guidedTime_++;
	if (guidedTime_ > coolTime_) {
		HomingUpdate();
		guidedTime_ = 0;
	}

	worldTransform_.translation_ += velocity_ * NLib::GetDeltaTime(60.0f);
	worldTransform_.UpdateMatrix();
}

void IMissile::Draw(ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection);
}

void IMissile::InitMoveParameter(const Vector3& direct, float speed)
{
	// 方向ベクトル
	direction_ = MathCalc::Normalize(direct);
	//SetDirection(direct);
	// 速さの大きさ
	kBulletSpeed_ = speed;
	//SetBulletSpeed(speed);
	// 移動ベクトルの初速度計算
	velocity_ = direct * speed;
	//velocity_ *= NLib::GetDeltaTime(60.0f);
}

void IMissile::SlerpUpdate()
{
	Vector3 toEnemy = target_->GetWorldPosition() - GetWorldPosition();

	toEnemy = MathCalc::Normalize(toEnemy);
	velocity_ = MathCalc::Normalize(velocity_);

	// 球面線形補間
	velocity_ = NLib::Slerp_Test(velocity_, toEnemy, 0.025f) * kBulletSpeed_;

}

void IMissile::ItanoUpdate()
{
	//Vector3 directToEnemy = target_->GetWorldPosition() - GetWorldPosition();
	//guidedTime_++;
	//if (guidedTime_ > 15) {
	//	guidedTime_ = 0;
	//}
	frame_ += 1.0f;
	//frame_ = std::clamp(frame_, 0.0f, 1.0f);
	Vector3 directToEnemy = target_->GetWorldPosition() - GetWorldPosition();
	p_ = NLib::InductionPerform(frame_, 0.01f);
	direction_ = NLib::InductionNewVector(direction_, directToEnemy, p_);
	acceleration_ = direction_ * NLib::GetDeltaTime(60.0f);
	velocity_ += acceleration_ * NLib::GetDeltaTime(60.0f);

}

void IMissile::TrackingMissileV1()
{
	Vector3 toTarget = target_->GetWorldPosition() - GetWorldPosition();
	Vector3 nowDirect = MathCalc::Normalize(velocity_);
	float dot = MathCalc::Dot(toTarget, nowDirect);
	Vector3 centripetalAccel = toTarget - (nowDirect * dot);
	float centripetalAccelMagnitude = MathCalc::Length(centripetalAccel);

	if (centripetalAccelMagnitude > 1.0f)
	{
		centripetalAccel /= centripetalAccelMagnitude;
	}

	//float rad = 15.0f;

	float maxCentripetalAccel = std::powf(kBulletSpeed_, 2) / lerpRad_;
	Vector3 force = centripetalAccel * maxCentripetalAccel;

	//float damping = 0.1f;
	float propulsion = kBulletSpeed_ * damping_;

	force += nowDirect * propulsion;
	force -= velocity_ * damping_;
	velocity_ += force * NLib::GetDeltaTime(60.0f);
}

void IMissile::HomingUpdate()
{
	switch (type_)
	{
	case MissileType::eSlerp:
		SlerpUpdate();
		break;
	case MissileType::eItano:
		ItanoUpdate();
		break;
	case MissileType::eV1:
		TrackingMissileV1();
		break;
	}
	//velocity_ *= NLib::GetDeltaTime(60.0f);
}

Vector3 IMissile::GetDeltaTimeVelocity()
{
	// TODO: return ステートメントをここに挿入します
	Vector3 newVelocity = velocity_ * NLib::GetDeltaTime(60.0f);
	return newVelocity;
}

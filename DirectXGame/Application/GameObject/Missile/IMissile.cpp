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
	ImGui::DragFloat3("velocity", &velocity_.x);
	ImGui::DragFloat("lerpRad", &lerpRad_, 0.1f, 0, 30.0f);
	ImGui::DragFloat("damping", &damping_, 0.01f, 0, 1.0f);
	ImGui::DragInt("coolTime", &coolTime_, 1, 0, 100);
	ImGui::DragFloat3("toEnemy", &toEnemy_.x);
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
	//Vector3 toEnemy = target_->GetWorldPosition() - GetWorldPosition();
	//toEnemy = MathCalc::Normalize(toEnemy);
	//velocity_ = MathCalc::Normalize(velocity_);

	//// 球面線形補間
	//velocity_ = NLib::Slerp_Test(velocity_, toEnemy, 0.025f) * kBulletSpeed_;

	toEnemy_ = target_->GetWorldPosition() - GetWorldPosition();
	toEnemy_ = MathCalc::Normalize(toEnemy_);
	velocity_ = MathCalc::Normalize(velocity_);

	velocity_ = NLib::Slerp_Test(velocity_, toEnemy_, 0.25f) * kBulletSpeed_;

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
		//ItanoUpdate();
		ProportionaMissile();
		break;
	case MissileType::eV1:
		TrackingMissileV1();
		break;
	}
	//velocity_ *= NLib::GetDeltaTime(60.0f);
}

void IMissile::ProportionaMissile()
{

	//float n = 0.01f; // 有効航法定数
	//float mu = 0.1f;	// 変化率

	//// 加速度 = 有効航法定数 * 速度 * 変化率
	//acceleration_ = velocity_ * mu * n;

#pragma region PNを使った基本的な加速度の求める計算
	//// 座標
	//Vector3 Rm = GetWorldPosition();
	//Vector3 Rt = target_->GetWorldPosition();

	//// 速度ベクトル
	//Vector3 Vm = velocity_;
	//Vector3 Vt = target_->GetVelocity();

	//Vector3 Vr = Vt - Vm;
	//float Ne = 20.0f;	// 有効航法定数

	//acceleration_ = Vr * Ne;

	//velocity_ += acceleration_ * NLib::GetDeltaTime(60.0f);
#pragma endregion

#pragma region ２つ目の計算案

	//// 座標
	//Vector3 Rm = GetWorldPosition();
	//Vector3 Rt = target_->GetWorldPosition();

	//// 速度ベクトル
	//Vector3 Vm = velocity_;
	//Vector3 Vt = target_->GetVelocity();

	//// 相対ベクトル
	//Vector3 Vr = Vt - Vm;

	//Vector3 R = Rt - Rm;	// LOSベクトル
	//float r = MathCalc::Length(R);	// 距離

	//Vector3 LOS_Rate = MathCalc::Cross(R, Vr) / r;

	//float Ne = 5.0f;	// 有効航法定数

	//acceleration_ = LOS_Rate * Ne;
	//velocity_ += acceleration_ * NLib::GetDeltaTime(60.0f);

#pragma endregion

#pragma region 3つ目の案

	//Vector3 relaVelocity = target_->GetVelocity() - velocity_;
	//Vector3 relaPosition = target_->GetWorldPosition() - GetWorldPosition();

	//float dot = MathCalc::Dot(relaVelocity, relaPosition);

	//Vector3 Vt = target_->GetVelocity();
	//Vector3 Vm = velocity_;

	//float crossValue = (Vt.x * Vm.x - Vt.y * Vm.y - Vt.z * Vm.z);

	//float angularPerm = crossValue + dot;

	//float Ne = 2.0f;

	//acceleration_ = (relaVelocity * Ne) * angularPerm;

	//velocity_ += acceleration_ * NLib::GetDeltaTime(60.0f);

#pragma endregion

#pragma region 4つ目の案

	Vector3 Vd = velocity_;
	Vector3 Vg = target_->GetWorldPosition();
	Vector3 Vp = GetWorldPosition();

	// 誘導係数
	float r = 0.5f;

	Vector3 V = Vg - Vp;

	V = MathCalc::Normalize(V);

	Vd = MathCalc::Normalize(Vd);

	Vector3 temp = Vd * r;

	Vector3 newVd = temp + (V * (1 - r));

	newVd = MathCalc::Normalize(newVd);

	float kSpeedValue = 8.0f;

	velocity_ += (newVd * kSpeedValue) * NLib::GetDeltaTime(60.0f);

	//target_->SetPosition(newVp);

	//velocity_ += acceleration_ * NLib::GetDeltaTime(60.0f);

#pragma endregion


}

Vector3 IMissile::GetDeltaTimeVelocity()
{
	// TODO: return ステートメントをここに挿入します
	Vector3 newVelocity = velocity_ * NLib::GetDeltaTime(60.0f);
	return newVelocity;
}

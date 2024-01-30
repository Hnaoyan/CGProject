#include "IMissile.h"
#include "NLib.h"
#include "imgui.h"
#include "MissileManager.h"
#include "WindowAPI.h"

#include <algorithm>

uint32_t IMissile::sSerialNumber_ = 1;

IMissile::IMissile()
{
	// 個体番号取得
	serialNum_ = sSerialNumber_;
	// 次の番号を加算
	++sSerialNumber_;
}

void IMissile::Initialize(Model* model, const Vector3& position)
{

	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.scale_ = { 0.25f,0.25f,0.25f };
	guidedTime_ = 0;
	slerp_t_ = WindowAPI::GetInstance()->GetRandom(0.1f, 0.45f);

	collider_.SetCollisionAttribute(kCollisionAttributeWeapon);
	collider_.SetRadius(1.0f);
	collider_.SetterRad({ 1,1,1 });
	collider_.SetWorldAddress(&worldTransform_);
	std::function<void(uint32_t, WorldTransform*)> f = std::function<void(uint32_t, WorldTransform*)>(std::bind(&IMissile::OnCollision, this, std::placeholders::_1, std::placeholders::_2));
	collider_.SetFunction(f);

}

void IMissile::Update()
{
	if (Input::GetInstance()->TriggerKey(DIK_L)) {
		target_ = nullptr;
	}
	
	HomingUpdate();

	if (guidedTime_ % 60 == 0) {
		manager_->AddParticle(GetWorldPosition(), 60);
	}

	velocity_ += acceleration_;

	if (type_ != kSlerp) {
		velocity_ * 2.0f;
	}

	worldTransform_.translation_ += GetDeltaTimeVelocity();

	Vector3 velocityVect = MathCalc::Normalize(velocity_);

	worldTransform_.rotation_.y = MathCalc::CalculateYawFromVector(Vector3(velocityVect.x, 0, velocityVect.z));
	//worldTransform_.rotation_.y = std::atan2f(velocity_.x, velocity_.z);
	float length = sqrtf(velocity_.x * velocity_.x + velocity_.z * velocity_.z);
	worldTransform_.rotation_.x = std::atan2f(-velocity_.y, length);

	worldTransform_.UpdateMatrix();
	collider_.SetPosition(worldTransform_.GetWorld());

	if (isFade_) {
		fadeTimer_--;
		//if (fadeTimer_ < 0) {
		//	fadeTimer_ = 0;
		//}
	}

}

void IMissile::Draw(ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection);
}

void IMissile::ImGuiUpdate()
{
	std::string tag = "M" + std::to_string(serialNum_);
	if (ImGui::BeginTabItem(tag.c_str())) {
		ImGui::Text(tag.c_str());
		ImGui::InputInt(tag.c_str(), &guidedTime_);
		ImGui::InputInt(tag.c_str(), &fadeTimer_);
		if (ImGui::TreeNode("Vector")) {
			ImGui::DragFloat3("toEnemy", &toEnemy_.x);
			ImGui::DragFloat3("velocity", &velocity_.x);
			ImGui::DragFloat3("accleration", &acceleration_.x);

			ImGui::TreePop();
		}
		ImGui::EndTabItem();
	}
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

	toEnemy_ = target_->GetWorldPosition() - GetWorldPosition();
	toEnemy_ = MathCalc::Normalize(toEnemy_);
	velocity_ = MathCalc::Normalize(velocity_);

	velocity_ = NLib::Slerp_Test(velocity_, toEnemy_, slerp_t_) * kBulletSpeed_;

}

void IMissile::ItanoUpdate()
{
	//Vector3 directToEnemy = target_->GetWorldPosition() - GetWorldPosition();
	//guidedTime_++;
	//if (guidedTime_ > 15) {
	//	guidedTime_ = 0;
	//}
	//frame_ += 1.0f;
	//frame_ = std::clamp(frame_, 0.0f, 1.0f);
	Vector3 directToEnemy = target_->GetWorldPosition() - GetWorldPosition();
	//p_ = NLib::InductionPerform(frame_, 0.01f);
	direction_ = NLib::InductionNewVector(direction_, directToEnemy, p_);
	acceleration_ = direction_ * NLib::GetDeltaTime(60.0f);
	velocity_ += acceleration_ * NLib::GetDeltaTime(60.0f);
}

void IMissile::HomingUpdate()
{
	coolTime_ = 10;
	isDelay_ = false;
	if (guidedTime_ > coolTime_) {
		isDelay_ = true;
		guidedTime_ = 0;
	}
	else {
		guidedTime_++;
	}

	int timer = 5;
	if (deathCount_ > (60 * timer)) {
		isDead_ = true;
	}

	if (GetWorldPosition().z > targetPosition_.z || target_ == nullptr) {
		deathCount_++;
		return;
	}

	switch (type_)
	{
	case MissileType::kSlerp:
		if (isDelay_) {
			SlerpUpdate();
		}
		break;
	case MissileType::kV1:
		TrackingMissileV1();
		break;
	case MissileType::kProt4:
		ProportionalV4();
		break;
	case MissileType::kProt6:
		ProportionalV6();
		break;
	case MissileType::kProt7:
		ProportionalV7();
		break;
	case MissileType::kV2:
		TrackingMissileV2();
		break;
	case MissileType::kNone:

		break;
	}
	//velocity_ *= NLib::GetDeltaTime(60.0f);
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

	float maxCentripetalAccel = std::powf(kBulletSpeed_, 2) / lerpRad_;
	Vector3 force = centripetalAccel * maxCentripetalAccel;

	float propulsion = kBulletSpeed_ * damping_;

	force += nowDirect * propulsion;
	force -= velocity_ * damping_;
	acceleration_ = force * NLib::GetDeltaTime(60.0f);
	//velocity_ += force * NLib::GetDeltaTime(60.0f);
}

void IMissile::TrackingMissileV2()
{
	cancelCount_++;
	int kCancelTime = 150;

	Vector3 toTarget = target_->GetWorldPosition() - GetWorldPosition();
	Vector3 nowDirect = MathCalc::Normalize(velocity_);
	float dot = MathCalc::Dot(toTarget, nowDirect);
	Vector3 centripetalAccel = toTarget - (nowDirect * dot);
	float centripetalAccelMagnitude = MathCalc::Length(centripetalAccel);

	if (centripetalAccelMagnitude > 1.0f)
	{
		centripetalAccel /= centripetalAccelMagnitude;
	}

	float maxCentripetalAccel = std::powf(kBulletSpeed_, 2) / lerpRad_;
	Vector3 force = centripetalAccel * maxCentripetalAccel;

	float propulsion = kBulletSpeed_ * damping_;

	force += nowDirect * propulsion;
	force -= velocity_ * damping_;
	acceleration_ = force * NLib::GetDeltaTime(60.0f);

	if (cancelCount_ > kCancelTime) {
		target_ = nullptr;
	}

}

void IMissile::ProportionaMissile()
{
#pragma region Prot1
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

#pragma region Prot2

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

#pragma region Prot3

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

#pragma region Prot4

	//ProportionalV4();

#pragma endregion

#pragma region Prot5

	//Vector3 R = target_->GetWorldPosition() - GetWorldPosition();
	//Vector3 V = target_->GetVelocity() - velocity_;

	////Vector3 cross = (V.x * R.x - V.y * R.y - V.z * R.z);

	//Vector3 rotVector = MathCalc::Cross(V,R) / MathCalc::Dot(R, R);

	//float constant = 30.0f;

	////velocity_ += VectorLib::Scaler(V, constant) * rotVector;
	//acceleration_ += VectorLib::Scaler(V, constant) * rotVector;
	//velocity_ += acceleration_ * NLib::DeltaTimeSpeed(60.0f);

#pragma endregion

#pragma region Prot6

	//ProportionalV6();

#pragma endregion

#pragma region Prot7

	ProportionalV7();

#pragma endregion

}

void IMissile::ProportionalV4()
{
#pragma region Prot4

	Vector3 Vd = velocity_;
	Vector3 Vg = target_->GetWorldPosition();
	Vector3 Vp = GetWorldPosition();

	// 誘導係数
	float r = 0.01f;

	Vector3 V = Vg - Vp;

	V = MathCalc::Normalize(V);

	Vd = MathCalc::Normalize(Vd);

	Vector3 temp = Vd * r;

	Vector3 newVd = temp + (V * (1 - r));

	newVd = MathCalc::Normalize(newVd);

	float kSpeedValue = 30.0f;

	velocity_ += (newVd * kSpeedValue) * NLib::GetDeltaTime(60.0f);

#pragma endregion
}

void IMissile::ProportionalV6()
{
#pragma region Prot6

	Vector3 M_Pos = GetWorldPosition();
	Vector3 T_Pos = target_->GetWorldPosition();

	Vector3 M_Velo = velocity_;
	Vector3 T_Velo = target_->GetVelocity();

	float distance = MathCalc::Length(M_Pos - T_Pos);
	if (distance < 0.0001f) {
		return;
	}

	Vector3 r = T_Pos - M_Pos;
	Vector3 v = T_Velo - M_Velo;
	float vc = (-MathCalc::Dot(r, v)) / MathCalc::Length(r);
	Vector3 crossR = MathCalc::Cross(r,v);

	//float lamd = std::atan2f(MathCalc::Length(crossR), MathCalc::Dot(r, v));
	float lamdDot = MathCalc::Dot(crossR, v) / (MathCalc::Length(r) * MathCalc::Length(r));

	float n = 3 * vc * lamdDot;

	//float n = 10.0f;

	acceleration_ = (crossR * (n / MathCalc::Length(r))) * NLib::GetDeltaTime(60.0f);
	velocity_ += acceleration_;

#pragma endregion

}

void IMissile::ProportionalV7()
{
#pragma region Prot7
	Vector3 targetDirection = (target_->GetWorldPosition() - GetWorldPosition());
	Vector3 currentDirection = MathCalc::Normalize(velocity_);

	// 誘導の割合を設定
	float proportionalGain = 0.3f;

	// 目標方向から少しオフセットされた方向を計算
	Vector3 newDirection = (targetDirection * (1 - proportionalGain)) + (currentDirection * proportionalGain);
	newDirection = MathCalc::Normalize(newDirection);

	// 初速度の大きさを保持したまま、目標方向に対する微調整を計算
	float initialSpeed = MathCalc::Length(velocity_);
	Vector3 adjustedAcceleration = (newDirection * initialSpeed) - velocity_;

	//// 最大調整角度を設定します（これにより小さな弧を描くように制御）
	//float maxAdjustmentAngle = 0.9f; // radians（適切な角度に調整）

	//// 新しい加速度が最大調整角度を超えないように制限
	//float currentAngle = acos(MathCalc::Dot(currentDirection, newDirection));
	//if (currentAngle > maxAdjustmentAngle) {
	//	float limitFactor = maxAdjustmentAngle / currentAngle;
	//	adjustedAcceleration = (newDirection - currentDirection) * (initialSpeed * limitFactor);
	//}
	acceleration_ = adjustedAcceleration * NLib::GetDeltaTime(60.0f);
	//// 速度を調整する係数を設定（0.1から1の間の値）
	//float speedAdjustmentFactor = 1.5f; // 適切な係数に調整

	//acceleration_ = adjustedAcceleration * speedAdjustmentFactor * NLib::GetDeltaTime(60.0f);
#pragma endregion
}

Vector3 IMissile::GetDeltaTimeVelocity()
{
	// TODO: return ステートメントをここに挿入します
	Vector3 newVelocity = velocity_ * NLib::GetDeltaTime(60.0f);
	return newVelocity;
}

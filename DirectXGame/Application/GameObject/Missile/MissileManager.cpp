#include "MissileManager.h"
#include "imgui.h"

void MissileManager::Initialize()
{

	model_.reset(Model::Create());

}

void MissileManager::Update()
{
#ifdef _DEBUG

	ImGui::Begin("missileManager");
	ImGui::DragFloat("kSpeed", &bulletSpeed_, 0.01f, 0, 50.0f);
	ImGui::End();

#endif // _DEBUG

	if (Input::GetInstance()->TriggerKey(DIK_R)) {
		missiles_.clear();
	}

	for (IMissile* missile : missiles_) {
		missile->Update();
	}

}

void MissileManager::Draw(ViewProjection& viewProjection)
{
	for (IMissile* missile : missiles_) {
		missile->Draw(viewProjection);
	}
}

void MissileManager::AddMissile(const MissileConfig info)
{
	IMissile* newInstance = new IMissile();
	newInstance->Initialize(model_.get(), info.position);
	//newInstance->SetDirection(MathCalc::Normalize(info.direct));
	//newInstance->SetBulletSpeed(bulletSpeed_);
	newInstance->InitMoveParameter(info.direct, info.kSpeed);
	newInstance->SetTarget(info.ptr);
	newInstance->SetType(info.type);
	missiles_.push_back(newInstance);
}

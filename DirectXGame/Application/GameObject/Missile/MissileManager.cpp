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

void MissileManager::AddMissile(const AddInfo info)
{
	IMissile* newInstance = new IMissile();
	newInstance->Initialize(model_.get());
	newInstance->SetDirection(MathCalc::Normalize(info.direct));
	newInstance->SetPosition(info.position);
	newInstance->SetBulletSpeed(bulletSpeed_);
	newInstance->SetTarget(info.ptr);
	missiles_.push_back(newInstance);
}

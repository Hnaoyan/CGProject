#include "MissileManager.h"
#include "imgui.h"

void MissileManager::Initialize()
{

	model_.reset(Model::CreateFromObj("Bullet",false));
	param_ = { 5.0f,0.1f };

}

void MissileManager::Update()
{
#ifdef _DEBUG

	ImGui::Begin("missileManager");
	ImGui::DragFloat("kSpeed", &bulletSpeed_, 0.01f, 0, 50.0f);
	ImGui::InputInt("MissileType", &missileType_);
	if (ImGui::TreeNode("MissileParam")) {
		ImGui::InputFloat("LerpRadius", &param_.lerpRad);
		ImGui::InputFloat("Damping", &param_.damping);
		ImGui::TreePop();
	}

	ImGui::End();

#endif // _DEBUG

	if (Input::GetInstance()->TriggerKey(DIK_R)) {
		missiles_.clear();
	}

	// 弾の消去
	missiles_.remove_if([](IMissile* missile) {
		if (missile->GetIsDead()) {
			delete missile;
			return true;
		}
		return false;
		});

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
	newInstance->InitMoveParameter(info.direct, bulletSpeed_);
	newInstance->SetTarget(info.ptr);
	newInstance->SetType(missileType_);
	newInstance->SetTargetPosition(info.ptr->GetWorldPosition());
	newInstance->SettingParameter(param_.lerpRad, param_.damping);
	newInstance->SetParticle(particleManager_);
	missiles_.push_back(newInstance);
}

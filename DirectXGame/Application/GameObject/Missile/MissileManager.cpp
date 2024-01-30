#include "MissileManager.h"
#include "imgui.h"
#include "EnemyManager.h"

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
	Vector3 TargetVect = enemyManager_->GetTestPtr()->GetWorldPosition();
	ImGui::DragFloat3("Pos", &TargetVect.x);
	ImGui::End();

	ImGuiUpdate();

#endif // _DEBUG

	if (Input::GetInstance()->TriggerKey(DIK_R)) {
		missiles_.clear();
	}

	//if (isSeparate_) {
	//	separateTime_++;
	//	if (separateTime_ > 60) {
	//		separateTime_ = 0;
	//		isSeparate_ = false;

	//		SilhouetteDance();

	//	}
	//}


	ListUpdate();
}

void MissileManager::Draw(ViewProjection& viewProjection)
{
	for (IMissile* missile : missiles_) {
		missile->Draw(viewProjection);
	}
}

void MissileManager::ListUpdate()
{

	// 弾の消去
	missiles_.remove_if([this](IMissile* missile) {
		if (/*missile->GetIsDead() &&*/ missile->GetIsFade()) {
			MissileConfig info = { missile->GetWorldPosition(),{},enemyManager_->GetTestPtr() };
			BurstTheGravity(info);
			delete missile;
			return true;
		}
		else if (missile->GetIsDead()) {
			delete missile;
			return true;
		}
		return false;
		});

	if (Input::GetInstance()->TriggerKey(DIK_J)) {
		isFrameStop_ = true;
	}
	if (Input::GetInstance()->TriggerKey(DIK_K)) {
		isFrameStop_ = false;
	}

	if (!isFrameStop_) {
		for (IMissile* missile : missiles_) {
			missile->Update();
		}
	}
}

void MissileManager::ImGuiUpdate()
{

	ImGui::Begin("MissileManager");
	if (ImGui::BeginTabBar("Missiles")) {

		for (IMissile* missile : missiles_) {
			missile->ImGuiUpdate();
		}

		ImGui::EndTabBar();
	}
	ImGui::End();

}

void MissileManager::AddMissile(const MissileConfig info)
{
	if (info.ptr == nullptr) {
		return;
	}
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

void MissileManager::AddMissileNoneType(const MissileConfig info, int type)
{
	IMissile* newInstance = new IMissile();
	newInstance->Initialize(model_.get(), info.position);
	newInstance->InitMoveParameter(info.direct, bulletSpeed_);
	newInstance->SetTarget(info.ptr);
	newInstance->SetType(type);
	newInstance->SetTargetPosition(info.ptr->GetWorldPosition());
	newInstance->SettingParameter(param_.lerpRad, param_.damping);
	newInstance->SetParticle(particleManager_);

	if (type == kV1) {
		newInstance->SetFade(100);
	}

	missiles_.push_back(newInstance);
}

void MissileManager::BurstTheGravity(const MissileConfig info)
{
 	if (info.ptr == nullptr) {
		return;
	}
	MissileConfig config = info;

#pragma region 上下左右
	// 上
	config.direct = { 0,1,0 };
	AddMissileNoneType(config, kSlerp);
	// 下
	config.direct = { 0,-1,0 };
	AddMissileNoneType(config, kSlerp);

	// 右
	config.direct = { 1,0,0 };
	AddMissileNoneType(config, kSlerp);
	// 左
	config.direct = { -1,0,0 };
	AddMissileNoneType(config, kSlerp);
#pragma endregion

#pragma region 斜め
	// 左上
	config.direct = { -1,1,0 };
	AddMissileNoneType(config, kProt4);
	// 左下
	config.direct = { -1,-1,0 };
	AddMissileNoneType(config, kProt4);

	// 右上
	config.direct = { 1,1,0 };
	AddMissileNoneType(config, kProt4);
	// 右下
	config.direct = { 1,-1,0 };
	AddMissileNoneType(config, kProt4);
#pragma endregion

}

void MissileManager::Ashen(const MissileConfig info)
{
	if (info.ptr == nullptr) {
		return;
	}

	isSeparate_ = true;

	MissileConfig config = info;
	// 左下
	config.direct = { -0.6f, -1.0f,0 };
	AddMissileNoneType(config, kV1);

	// 右下
	config.direct = { 0.6f, -1.0f,0 };
	AddMissileNoneType(config, kV1);

	targetPtr_ = config.ptr;

}

void MissileManager::SilhouetteDance()
{
	if (targetPtr_ == nullptr) {
		return;
	}

	MissileConfig config;

	config.ptr = targetPtr_;
	config.position = playerPosition_;
	config.direct = {};

	BurstTheGravity(config);

	targetPtr_ = nullptr;

}

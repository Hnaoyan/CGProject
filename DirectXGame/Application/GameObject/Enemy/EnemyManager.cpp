#include "EnemyManager.h"
#include "imgui.h"

void EnemyManager::Initialize()
{

	model_.reset(Model::CreateFromObj("enemy", true));

	testEnemy_ = std::make_unique<Enemy>();
	testEnemy_->Initialize(model_.get());
	testEnemy_->SetPosition({ 0,0,35.0f });

	hitPoint_ = 20;

}

void EnemyManager::Update()
{
	// テスト用
	testEnemy_->Update();

	// リスト
	for (Enemy* enemy : enemys_) {
		enemy->Update();
	}

	if (testEnemy_->GetIsHit()) {
		hitPoint_--;
		testEnemy_->SetIsHit(false);
	}

#ifdef _DEBUG
	ImGui::Begin("EManager");

	ImGui::Text("HP : %d", hitPoint_);

	ImGui::End();

#endif // _DEBUG


}

void EnemyManager::Draw(ViewProjection& viewProjection)
{
	testEnemy_->Draw(viewProjection);

	// リスト
	for (Enemy* enemy : enemys_) {
		enemy->Draw(viewProjection);
	}
}

void EnemyManager::AddEnemy(const AddInfo info)
{
	Enemy* newInstance = new Enemy();
	newInstance->Initialize(model_.get());
	newInstance->SetPosition(info.position);
	enemys_.push_back(newInstance);
}

#include "EnemyManager.h"

void EnemyManager::Initialize()
{

	model_.reset(Model::Create());
	testEnemy_ = std::make_unique<Enemy>();
	testEnemy_->Initialize(model_.get());
	testEnemy_->SetPosition({ 0,0,50.0f });
}

void EnemyManager::Update()
{
	// テスト用
	testEnemy_->Update();

	// リスト
	for (Enemy* enemy : enemys_) {
		enemy->Update();
	}

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

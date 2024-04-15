#include "GameScene.h"
#include "TextureManager.h"
#include "ImGuiManager.h"
#include <cassert>

GameScene::GameScene() 
{
}

GameScene::~GameScene()
{
}

void GameScene::Initialize() {

	IScene::Initialize();
}

void GameScene::Update()
{

}

void GameScene::Draw() {

}


void GameScene::CameraUpdate()
{

}

void GameScene::CheckCollision()
{
	// 初期化
	colliderManager_->ResetList();

	//// 追加
	//colliderManager_->AddList(player_->GetCollider());
	////if (player_->GetIsAttack()) {
	////	colliderManager_->AddList(player_->GetWeapon());
	////}
	//if (!enemy_->GetIsDead()) {
	//	colliderManager_->AddList(enemy_->GetCollider());
	//}
	/// 当たり判定（仮
	colliderManager_->CheckAllCollisions();
}
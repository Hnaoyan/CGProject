#include "GameScene.h"
#include "TextureManager.h"
#include "ImGuiManager.h"
#include <cassert>

GameScene::GameScene() 
{
	model_.reset(Model::Create());
}

GameScene::~GameScene()
{
}

void GameScene::Initialize() {
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	viewProjection_.Initialize();

	// 衝突マネージャー
	colliderManager_ = std::make_unique<CollisionManager>();

	// 追従カメラ
	followCamera_ = std::make_unique<FollowCamera>();
	followCamera_->Initialize();

}

void GameScene::Update()
{
	// 衝突処理
	CheckCollision();

	/// カメラ関係の更新処理
	CameraUpdate();

}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 描画前処理
	Sprite::PreDraw(commandList);


	// 描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 描画前処理
	Model::PreDraw(commandList);



	// 描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 描画前処理
	Sprite::PreDraw(commandList);



	// 描画後処理
	Sprite::PostDraw();

#pragma endregion
}


void GameScene::CameraUpdate()
{
#ifdef _DEBUG
	if (input_->TriggerKey(DIK_L)) {
		if (isDebug_) {
			isDebug_ = false;
		}
		else {
			isDebug_ = true;
		}
	}
#endif // DEBUG

	followCamera_->Update();

	// デバックカメラか追尾カメラ
	if (isDebug_) {

	}
	else {
		viewProjection_.matView = followCamera_->GetView().matView;
		viewProjection_.matProjection = followCamera_->GetView().matProjection;
		viewProjection_.TransferMatrix();
	}
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
	//colliderManager_->AddList(goal_.get());

	//for (Ground* ground : groundManager_->GetList()) {
	//	colliderManager_->AddList(ground->GetCollider());
	//}


	/// 当たり判定（仮
	colliderManager_->CheckAllCollisions();
}
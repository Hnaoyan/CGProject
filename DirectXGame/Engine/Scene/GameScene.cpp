#include "GameScene.h"
#include "TextureManager.h"
#include "ImGuiManager.h"
#include <cassert>

GameScene::GameScene() {}

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

	baseCamera_ = std::make_unique<BaseCamera>();
	baseCamera_->Initialize();

	baseCamera_->SetPosition({ 0, 10.0f, -25.0f });
	baseCamera_->SetRotation({ 0.3f, 0, 0 });

	skydomeModel_.reset(Model::CreateFromObj("skydome", true));
	skydome_ = std::make_unique<SkyDome>();
	skydome_->Initialize(skydomeModel_.get());

	model_.reset(Model::Create());
	player_ = std::make_unique<Player>();
	player_->Initialize(model_.get());
	followCamera_->SetTarget(player_->GetWorldTransform());

	modelBody_.reset(Model::CreateFromObj("C_Body", true));
	modelL_arm_.reset(Model::CreateFromObj("C_Left", true));
	modelR_arm_.reset(Model::CreateFromObj("C_Right", true));

	std::vector<Model*> models =
	{ modelBody_.get(),modelL_arm_.get(),modelR_arm_.get() };

	enemy_ = std::make_unique<Enemy>();
	enemy_->Initialize(model_.get());
	enemy_->SetModel(models);

	goalModel_.reset(Model::Create());
	goal_ = std::make_unique<Goal>();
	goal_->Initialize(goalModel_.get());

	groundManager_ = std::make_unique<GroundManager>();
	groundManager_->Initialize();
	groundManager_->SetManager(colliderManager_.get());

	Vector3 groundRad = { 10.0f,0.2f,10.0f };

	groundManager_->AddGround(Vector3(0, -0.2f, 0.0f), groundRad, Vector3(2.0f,1.0f,2.0f));
	groundManager_->AddGround(Vector3(0, -0.2f, 40.0f), groundRad,Vector3(2.0f,1.0f,2.0f));
	groundManager_->AddMoveGround(Vector3(0, -0.4f, 20.0f),
		Vector3(5.0f, groundRad.y, 5.0f), Vector3(1.0f, 1.0f, 1.0f));

}

void GameScene::Update()
{

	if (player_->GetIsDead()) {
		player_->DeadToRestart(Vector3(0,1.0f,0));
	}


	groundManager_->Update();
	skydome_->Update();
	goal_->Update();

	player_->Update();

	enemy_->Update();
	CheckCollision();


	/// カメラ関係の更新処理
	CameraUpdate();

}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>


	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	
	player_->Draw(viewProjection_);
	enemy_->Draw(viewProjection_);

	goal_->Draw(viewProjection_);
	skydome_->Draw(viewProjection_);
	groundManager_->Draw(viewProjection_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
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

	baseCamera_->Update();
	followCamera_->Update();

	// デバックカメラか追尾カメラ
	if (isDebug_) {

	}
	else {
		//viewProjection_.matView = baseCamera_->GetView().matView;
		//viewProjection_.matProjection = baseCamera_->GetView().matProjection;
		//viewProjection_.TransferMatrix();
		viewProjection_.matView = followCamera_->GetView().matView;
		viewProjection_.matProjection = followCamera_->GetView().matProjection;
		viewProjection_.TransferMatrix();
	}
}

void GameScene::CheckCollision()
{
	colliderManager_->ResetList();

	colliderManager_->AddList(player_->GetCollider());
	colliderManager_->AddList(enemy_->GetCollider());
	colliderManager_->AddList(goal_.get());

	for (Ground* ground : groundManager_->GetList()) {
		colliderManager_->AddList(ground->GetCollider());
	}

	/// 当たり判定（仮
	colliderManager_->CheckAllCollisions();
}

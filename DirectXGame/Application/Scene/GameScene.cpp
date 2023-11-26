#include "GameScene.h"
#include "TextureManager.h"
#include "ImGuiManager.h"
#include <cassert>

GameScene::GameScene() 
{
	model_.reset(Model::Create());
	modelBody_.reset(Model::CreateFromObj("C_Body", true));
	modelL_arm_.reset(Model::CreateFromObj("C_Left", true));
	modelR_arm_.reset(Model::CreateFromObj("C_Right", true));
	modelWeapon_.reset(Model::CreateFromObj("hammer", true));
	goalModel_.reset(Model::Create());
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

	skydomeModel_.reset(Model::CreateFromObj("skydome", true));
	skydome_ = std::make_unique<SkyDome>();
	skydome_->Initialize(skydomeModel_.get());


	std::vector<Model*> models =
	{ modelBody_.get(),modelL_arm_.get(),modelR_arm_.get(),modelWeapon_.get()};

	player_ = std::make_unique<Player>();
	player_->Initialize(models);
	followCamera_->SetTarget(player_->GetWorldTransform());
	player_->SetViewProjection(followCamera_->GetViewPlayer());

	enemy_ = std::make_unique<Enemy>();
	enemy_->Initialize(model_.get());
	enemy_->SetModel(models);



	//for (int i = 0; i < 1; i++) {
	//	std::unique_ptr<Enemy> enemy = std::make_unique<Enemy>();
	//	enemy->Initialize(model_.get());
	//	enemy->SetModel(models);
	//	enemy->SetPosition(Vector3{ float(3 * i),0.0f,45.0f + (5 * i)});
	//	enemies_.push_back(std::move(enemy));
	//}
	
	std::unique_ptr<Enemy> enemy;
	enemy = std::make_unique<Enemy>();
	enemy->Initialize(model_.get());
	enemy->SetModel(models);
	enemy->SetPosition(Vector3{ 0,0,50.0f });
	enemies_.push_back(std::move(enemy));
	enemy = std::make_unique<Enemy>();
	enemy->Initialize(model_.get());
	enemy->SetModel(models);
	enemy->SetPosition(Vector3{ 0,0,-50.0f });
	enemies_.push_back(std::move(enemy));
	enemy = std::make_unique<Enemy>();
	enemy->Initialize(model_.get());
	enemy->SetModel(models);
	enemy->SetPosition(Vector3{ 50.0f,0,0 });
	enemies_.push_back(std::move(enemy));
	enemy = std::make_unique<Enemy>();
	enemy->Initialize(model_.get());
	enemy->SetModel(models);
	enemy->SetPosition(Vector3{ -50.0f,0,0 });
	enemies_.push_back(std::move(enemy));


	lockOn_ = std::make_unique<LockOn>();
	lockOn_->Initialize();
	//followCamera_->SetLockOn(lockOn_.get());

	goal_ = std::make_unique<Goal>();
	goal_->Initialize(goalModel_.get());

	groundManager_ = std::make_unique<GroundManager>();
	groundManager_->Initialize();
	groundManager_->SetManager(colliderManager_.get());

	Vector3 groundRad = { 5.0f,0.2f,5.0f };
	float scale = 10.0f;
	groundRad.x *= scale;
	groundRad.z *= scale;

	groundManager_->AddGround(Vector3(0, -0.2f, 10.0f), groundRad, Vector3(scale, 1.0f, scale));
	//groundManager_->AddGround(Vector3(0, -0.2f, 40.0f), groundRad, Vector3(4.0f, 1.0f, 4.0f));
	//groundManager_->AddGround(Vector3(0, -0.2f, 70.0f), groundRad, Vector3(4.0f, 1.0f, 4.0f));

	//groundManager_->AddMoveGround(Vector3(0, -0.4f, 20.0f),
	//	Vector3(5.0f, groundRad.y, 5.0f), Vector3(1.0f, 1.0f, 1.0f));

	groundManager_->Update();
}

void GameScene::Update()
{
	// 衝突処理
	CheckCollision();


	if (player_->GetIsDead()) {
		player_->DeadToRestart(Vector3(0, 1.0f, 0));
	}

	groundManager_->Update();
	skydome_->Update();
	goal_->Update();

	player_->Update();

	//for (Enemy* enemy : enemies_) {
	//	enemy->Update();
	//}

	for (auto itr = enemies_.begin(), end_ = enemies_.end(); itr != end_; itr++) {
		itr->get()->Update();
	}


	/// カメラ関係の更新処理
	CameraUpdate();

	lockOn_->Update(enemies_, viewProjection_);

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

	player_->Draw(viewProjection_);
	//if (!enemy_->GetIsDead()) {
	//	enemy_->Draw(viewProjection_);
	//}
	for (auto itr = enemies_.begin(), end_ = enemies_.end(); itr != end_; itr++) {
		itr->get()->Draw(viewProjection_);
	}
	//for (Enemy* enemy : enemies_) {
	//	enemy->Draw(viewProjection_);
	//}

	goal_->Draw(viewProjection_);
	skydome_->Draw(viewProjection_);
	groundManager_->Draw(viewProjection_);

	// 描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 描画前処理
	Sprite::PreDraw(commandList);

	lockOn_->Draw();


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

	// 追加
	colliderManager_->AddList(player_->GetCollider());
	if (player_->GetIsAttack()) {
		colliderManager_->AddList(player_->GetWeapon());
	}
	if (!enemy_->GetIsDead()) {
		colliderManager_->AddList(enemy_->GetCollider());
	}
	colliderManager_->AddList(goal_.get());

	for (Ground* ground : groundManager_->GetList()) {
		colliderManager_->AddList(ground->GetCollider());
	}


	/// 当たり判定（仮
	colliderManager_->CheckAllCollisions();
}
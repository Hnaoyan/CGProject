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

	particleManager_ = std::make_unique<ParticleManager>();
	particleManager_->Initialize();

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
	player_->SetPariticleManager(particleManager_.get());

	lockOn_ = std::make_unique<LockOn>();
	lockOn_->Initialize();
	followCamera_->SetLockOn(lockOn_.get());
	player_->SetLockOn(lockOn_.get());

	goal_ = std::make_unique<Goal>();
	goal_->Initialize(goalModel_.get());

	groundManager_ = std::make_unique<GroundManager>();
	groundManager_->Initialize();
	groundManager_->SetManager(colliderManager_.get());

	Vector3 groundRad = { 5.0f,0.2f,5.0f };
	float scale = 20.0f;
	groundRad.x *= scale;
	groundRad.z *= scale;

	groundManager_->AddGround(Vector3(0, -0.2f, 10.0f), groundRad, Vector3(scale, 1.0f, scale));

	groundManager_->Update();

	EnemySpawn();

}

void GameScene::Update()
{
	// 衝突処理
	CheckCollision();


	if (player_->GetIsDead()) {
		enemies_.clear();
		player_->DeadToRestart(Vector3(0, 1.0f, 0));
		EnemySpawn();
	}

	groundManager_->Update();
	skydome_->Update();
	goal_->Update();

	player_->Update();
	particleManager_->Update();

	for (auto itr = enemies_.begin(), end_ = enemies_.end(); itr != end_; itr++) {
		itr->get()->Update();
	}

	enemies_.remove_if([](std::unique_ptr<Enemy>& enemy) {
		if (enemy->GetIsDead()) {
			return true;
		}
		return false;
		});

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

	player_->Draw(viewProjection_);

	for (auto itr = enemies_.begin(), end_ = enemies_.end(); itr != end_; itr++) {
		itr->get()->Draw(viewProjection_);
	}

	goal_->Draw(viewProjection_);
	skydome_->Draw(viewProjection_);
	groundManager_->Draw(viewProjection_);
	particleManager_->Draw(viewProjection_);

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

	lockOn_->Update(enemies_, followCamera_->GetView());

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

	for (auto itr = enemies_.begin(), end_ = enemies_.end(); itr != end_; itr++) {
		if (!itr->get()->GetIsDead()) {
			colliderManager_->AddList(itr->get()->GetCollider());
		}
	}

	colliderManager_->AddList(goal_.get());

	for (Ground* ground : groundManager_->GetList()) {
		colliderManager_->AddList(ground->GetCollider());
	}


	/// 当たり判定（仮
	colliderManager_->CheckAllCollisions();
}

void GameScene::EnemySpawn()
{
	std::vector<Model*> models =
	{ modelBody_.get(),modelL_arm_.get(),modelR_arm_.get(),modelWeapon_.get() };

#pragma region Enemy
	std::unique_ptr<Enemy> enemy;
	// 1
	enemy = std::make_unique<Enemy>();
	enemy->Initialize(model_.get());
	enemy->SetModel(models);
	enemy->SetPosition(Vector3{ 0,0,50.0f });
	enemy->UpdateCollider();
	enemy->SetWeapon(player_->GetWeapon());
	enemies_.push_back(std::move(enemy));
	// 2
	enemy = std::make_unique<Enemy>();
	enemy->Initialize(model_.get());
	enemy->SetModel(models);
	enemy->SetPosition(Vector3{ 0,0,-50.0f });
	enemy->UpdateCollider();
	enemy->SetWeapon(player_->GetWeapon());
	enemies_.push_back(std::move(enemy));
	// 3
	enemy = std::make_unique<Enemy>();
	enemy->Initialize(model_.get());
	enemy->SetModel(models);
	enemy->SetPosition(Vector3{ 50.0f,0,0 });
	enemy->UpdateCollider();
	enemy->SetWeapon(player_->GetWeapon());
	enemies_.push_back(std::move(enemy));
	// 4
	enemy = std::make_unique<Enemy>();
	enemy->Initialize(model_.get());
	enemy->SetModel(models);
	enemy->SetPosition(Vector3{ -50.0f,0,0 });
	enemy->UpdateCollider();
	enemy->SetWeapon(player_->GetWeapon());
	enemies_.push_back(std::move(enemy));
#pragma endregion

}

#include "GameScene.h"
#include "TextureManager.h"
#include "ImGuiManager.h"
#include <cassert>

GameScene::GameScene() 
{
	model_.reset(Model::Create());
	//model_.reset(Model::CreateFromObj("Bullet", false));
}

GameScene::~GameScene()
{
}

void GameScene::Initialize() {
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	viewProjection_.Initialize();

	model_.reset(Model::CreateFromObj("Jett", true));
	skyDomeModel_.reset(Model::CreateFromObj("skydome", true));

	// プレイヤー
	player_ = std::make_unique<Player>();
	player_->Initialize(model_.get());

	enemyManager_ = std::make_unique<EnemyManager>();
	enemyManager_->Initialize();


	// 衝突マネージャー
	colliderManager_ = std::make_unique<CollisionManager>();

	fixedPointCamera_ = std::make_unique<FixedPointCam>();
	fixedPointCamera_->Initialize();

	viewProjection_.translate_ = { 0,6.0f,-75.0f };
	missileManager_ = std::make_unique<MissileManager>();
	missileManager_->Initialize();

	player_->SetEnemyManager(enemyManager_.get());
	player_->SetMissileManager(missileManager_.get());



	skyDome_ = std::make_unique<SkyDome>();
	skyDome_->Initialize(skyDomeModel_.get());

	particleManager_ = std::make_unique<ParticleManager>();
	particleManager_->Initialize();

	missileManager_->SetParticleManager(particleManager_.get());
}

void GameScene::Update()
{
#ifdef _DEBUG
	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		DEVMODE dm;
		ZeroMemory(&dm, sizeof(dm));
		dm.dmSize = sizeof(dm);

		if (EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &dm) != 0) {
			frame_ = static_cast<float>(dm.dmDisplayFrequency);
		}
	}
	ImGui::Begin("test");
	ImGui::Text("MaxFPS : %f", frame_);
	ImGui::End();
#endif // _DEBUG

	particleManager_->Update();
	particleManager_->SetView(&viewProjection_);

	// 衝突処理
	CheckCollision();

	player_->Update();
	enemyManager_->Update();
	missileManager_->Update();

	if (input_->TriggerKey(DIK_R)) {
		//particleManager_->AddParticle(player_->GetWorldPosition(), 30, &viewProjection_);
	}

	/// カメラ関係の更新処理
	CameraUpdate();

	skyDome_->Update();

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

	skyDome_->Draw(viewProjection_);

	enemyManager_->Draw(viewProjection_);
	player_->Draw(viewProjection_);
	missileManager_->Draw(viewProjection_);

	particleManager_->Draw(viewProjection_);

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

	fixedPointCamera_->Update();


	// デバックカメラ
	if (isDebug_) {

	}
	else {
		viewProjection_.matView = fixedPointCamera_->GetView().matView;
		viewProjection_.matProjection = fixedPointCamera_->GetView().matProjection;
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
	colliderManager_->AddList(enemyManager_->GetTestPtr()->GetCollider());

	for (IMissile* missile : missileManager_->GetList()) {
		colliderManager_->AddList(missile->GetCollider());
	}

	/// 当たり判定（仮
	colliderManager_->CheckAllCollisions();
}
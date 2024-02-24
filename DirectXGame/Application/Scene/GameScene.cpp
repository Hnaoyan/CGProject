#include "GameScene.h"
#include "TextureManager.h"
#include "ImGuiManager.h"
#include <cassert>

GameScene::GameScene() 
{
	model_.reset(Model::Create());
	planeModel_.reset(Model::CreatePlane());
	//model_.reset(Model::CreateFromObj("Bullet", false));
}

GameScene::~GameScene()
{
}

void GameScene::Initialize() {

	IScene::Initialize();

	model_.reset(Model::CreateFromObj("Jett", true));
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

	particleManager_ = std::make_unique<ParticleManager>();
	particleManager_->Initialize();

	missileManager_->SetParticleManager(particleManager_.get());
	missileManager_->SetEnemyPtr(enemyManager_.get());

	uint32_t texture = TextureManager::Load("UI/BackGround.png");
	back_.reset(Sprite::Create(texture, { 0,0 }, { 1,1,1,1 }, { 0,0 }, false, false));

	texture = TextureManager::Load("UI/Shot_B.png");
	uiInfo_.position = { 80,620 };
	uiInfo_.color = { 1,1,1,1 };
	uiInfo_.anchor = { 0.5f,0.5f };

	ui_.reset(Sprite::Create(texture, uiInfo_.position, uiInfo_.color, uiInfo_.anchor, false, false));

	planeWTF_.Initialize();
	planeWTF_.translation_.y = -30.0f / 2;
	planeWTF_.scale_ = { 50.0f,50.0f,1.0f };
	planeWTF_.rotation_.x = 1.57f;
	planeWTF_.UpdateMatrix();
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

	ImGui::DragFloat3("positionPlane", &planeWTF_.translation_.x, 0.01f);
	ImGui::DragFloat3("scalePlane", &planeWTF_.scale_.x, 0.01f);
	ImGui::DragFloat3("RotatePlane", &planeWTF_.rotation_.x,0.01f);

	ImGui::End();
#endif // _DEBUG
	planeWTF_.UpdateMatrix();
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

	if (enemyManager_->GetHP() <= 0) {
		sceneNum++;
	}

}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 描画前処理
	Sprite::PreDraw(commandList);

	back_->Draw();

	// 描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 描画前処理
	Model::PreDraw(commandList);


	enemyManager_->Draw(viewProjection_);
	player_->Draw(viewProjection_);
	missileManager_->Draw(viewProjection_);

	planeModel_->Draw(planeWTF_, viewProjection_);

	particleManager_->Draw(viewProjection_);

	// 描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 描画前処理
	Sprite::PreDraw(commandList);

	ui_->Draw();

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
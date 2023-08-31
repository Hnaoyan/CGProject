#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	viewProjection_.Initialize();

	// プレイヤー
	player_ = std::make_unique<Player>();
	plModel_.reset(Model::CreateFromObj("player", true));
	player_->Initialize(plModel_.get());
	plBulletModel_.reset(Model::CreateFromObj("bullet", true));
	player_->SetBulletModel(plBulletModel_.get());

	// エネミー
	eneModel_.reset(Model::CreateFromObj("enemy", true));

	// 地面
	ground_ = std::make_unique<Ground>();
	groundModel_.reset(Model::CreateFromObj("ground", true));
	ground_->Initialize(groundModel_.get());

	skyDome_ = std::make_unique<Skydome>();
	skyDomeModel_.reset(Model::CreateFromObj("skydome", true));
	skyDome_->Initialize(skyDomeModel_.get());

	// 追従カメラ
	followCamera_ = std::make_unique<FollowCamera>();
	followCamera_->Initialize();
	followCamera_->SetTarget(player_->GetWorldTransformTarget());

	uint32_t infoTex = TextureManager::Load("gameScene/gameInfo.png");
	info_ = Sprite::Create(infoTex, Vector2(0, 0), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0, 0), false, false);

	uint32_t textureTitle = TextureManager::Load("gameScene/clearFont.png");
	clear_ = Sprite::Create(textureTitle, { 480 ,60 }, { 1.0f,1.0f,1.0f,1.0f }, { 0.0f,0.0f }, false, false);

	for (int i = 0; i < 5; i++) {
		Enemy* newEnemy = new Enemy();
		newEnemy->Initialize(eneModel_.get());
		newEnemy->SetPosition(Vector3(7.5f * i, 3.0f, 25.0f));
		enemys_.push_back(newEnemy);
	}
	this->enemyCount_ = 0;
	stageNum = StageNumber::kStageOne;
	clearCount_ = 0;
}

void GameScene::Update() {
	ground_->Update();
	skyDome_->Update();

	// プレイヤー処理
	player_->Update();

	if (enemyCount_ > 4 && stageNum == StageNumber::kStageOne) {
		stageNum = StageNumber::kStageTwo;
		for (int i = 0; i < 4; i++) {
			Enemy* newEnemy = new Enemy();
			newEnemy->Initialize(eneModel_.get());
			newEnemy->SetPosition(Vector3(10.0f * i, 24.0f, 33.5f));
			enemys_.push_back(newEnemy);
		}
	}
	else if (enemyCount_ > 8 && stageNum == StageNumber::kStageTwo) {
		stageNum = StageNumber::kStageThree;
		for (int i = 0; i < 4; i++) {
			Enemy* newEnemy = new Enemy();
			newEnemy->Initialize(eneModel_.get());
			newEnemy->SetPosition(Vector3(15.0f * i, 24.0f, 33.5f));
			enemys_.push_back(newEnemy);
		}
		for (int i = -3; i < 3; i++) {
			Enemy* newEnemy = new Enemy();
			newEnemy->Initialize(eneModel_.get());
			newEnemy->SetPosition(Vector3(12.5f * i, 3.0f, 40.5f + (i * 2.0f)));
			enemys_.push_back(newEnemy);
		}
	}
	else if (enemyCount_ > 18 && stageNum == StageNumber::kStageThree) {
		stageNum = StageNumber::kStageClear;
	}


	if (stageNum == StageNumber::kStageClear) {
		clearCount_ += 1;
		if (clearCount_ == 200) {
			sceneNum = Scene::TITLE;
		}
	}


	// エネミー処理
	enemys_.remove_if([](Enemy* enemy) {
		if (enemy->IsDead()) {
			delete enemy;
			return true;
		}
		return false;
	});

	for (Enemy* enemy : enemys_) {
		enemy->Update();
	}
	CheckAllCollision();

	// 追尾カメラ処理
	followCamera_->SetRotate(player_->GetWorldTransform().rotation_);
	followCamera_->Update();

#ifdef DEBUG
	if (input_->TriggerKey(DIK_L)) {
		if (isDebug_) {
			isDebug_ = false;
		}
		else {
			isDebug_ = true;
		}
	}
#endif // DEBUG

	// デバックカメラか追尾カメラ
	if (isDebug_) {

	}
	else {
		viewProjection_.matView = followCamera_->GetView().matView;
		viewProjection_.matProjection = followCamera_->GetView().matProjection;
		viewProjection_.TransferMatrix();
	}

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

	for (Enemy* enemy : enemys_) {
		enemy->Draw(viewProjection_);
	}

	ground_->Draw(viewProjection_);

	skyDome_->Draw(viewProjection_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>
	info_->Draw();
	if (stageNum == StageNumber::kStageClear) {
		clear_->Draw();
	}


	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::CheckAllCollision()
{
	Vector3 posA, posB;

	const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();

	for (Enemy* enemy : enemys_) {
		// 敵キャラの座標
		posA = enemy->GetWorldTransform().translation_;
		// 敵キャラと自弾全ての当たり判定
		for (PlayerBullet* bullet : playerBullets) {
			// 弾の座標
			posB = bullet->GetWorldPosition();

			float distance = {
			powf(posA.x - posB.x, 2) + powf(posA.y - posB.y, 2) + powf(posA.z - posB.z, 2) };
			float radius = 2.0f + bullet->radius;
			// 交差判定
			if (distance <= radius) {
				// 敵キャラの衝突時のコールバック
				enemy->OnCollision();
				// 自弾の衝突時のコールバック
				bullet->OnCollision();
				// 敵の数カウント
				this->enemyCount_ += 1;
			}
		}
	}
}

void GameScene::CheckGroundToPlayer()
{

}

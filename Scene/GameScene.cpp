#include "GameScene.h"
#include "TextureManager.h"
//#include "AxisIndicator.h"
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
	plBulletModel_.reset(Model::CreateFromObj("cube", true));
	player_->SetBulletModel(plBulletModel_.get());

	// エネミー
	enemy_ = std::make_unique<Enemy>();
	eneModel_.reset(Model::CreateFromObj("enemy", true));
	enemy_->Initialize(eneModel_.get());

	// 地面
	ground_ = std::make_unique<Ground>();
	//groundModel_.reset(Model::CreateFromObj("ground", true));
	//ground_->Initialize(groundModel_.get());

	// 追従カメラ
	followCamera_ = std::make_unique<FollowCamera>();
	followCamera_->Initialize();
	followCamera_->SetTarget(player_->GetWorldTransformTarget());

}

void GameScene::Update() {

	//ground_->Update();

	// プレイヤー処理
	player_->Update();

	// エネミー処理
	enemy_->Update();

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

	enemy_->Draw(viewProjection_);

	//ground_->Draw(viewProjection_);

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

void GameScene::CheckAllCollision()
{
	Vector3 posA, posB;

	const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();

	//for (Enemy* enemy : enemys_) {
		// 敵キャラの座標
	posA = enemy_->GetWorldTransform().translation_;
	// 敵キャラと自弾全ての当たり判定
	for (PlayerBullet* bullet : playerBullets) {
		// 弾の座標
		posB = bullet->GetWorldPosition();

		float distance = {
			powf(posA.x - posB.x, 2) + powf(posA.y - posB.y, 2) + powf(posA.z - posB.z, 2) };
		float radius = 5.0f + bullet->radius;
		// 交差判定
		if (distance <= radius) {
			// 敵キャラの衝突時のコールバック
			enemy_->OnCollision();
			// 自弾の衝突時のコールバック
			bullet->OnCollision();
		}
	}
	//}
}

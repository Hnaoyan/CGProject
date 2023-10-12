#include "BossEnemy.h"
#include "Application/GameObject/BlockManager/BlockManager.h"
#include <GlobalVariables.h>

BossEnemy::~BossEnemy()
{
	
	//timedCall_の解放
	for (TimedCall* timedCall : timedCalls_) {
		delete timedCall;
	}

}

void BossEnemy::Initialize(Model* model, BlockManager* blockManager, EffectManager* effectManager)
{

	// ワールドトランスフォーム
	worldTransform_.Initialize();

	// モデル
	model_ = model;

	// エフェクトマネージャー
	effectManager_ = effectManager;

	// ブロックマネージャー
	blockManager_ = blockManager;

	// エネミーの攻撃関数
	enemyAttackFunc_ = std::function<void()>(std::bind(&BlockManager::EnemyAttackBlockGenerate, blockManager_));

	//コールバック設定
	std::function<void(uint32_t, WorldTransform*)> f = std::function<void(uint32_t, WorldTransform*)>(std::bind(&BossEnemy::OnCollision, this, std::placeholders::_1, std::placeholders::_2));
	collider_.SetFunction(f);

#pragma region 調整項目クラス
	// 調整項目クラスのインスタンス取得
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	// グループ名設定
	const char* groupName = "BossEnemy";
	// 指定した名前でグループ追加
	globalVariables->CreateGroup(groupName);

	// メンバ変数の調整したい項目をグローバル変数に追加

	globalVariables->AddItem(groupName, "kDropBlockInterval_", kDropBlockInterval_);
	globalVariables->AddItem(groupName, "kInitialHp_", static_cast<int>(kInitialHp_));
	globalVariables->AddItem(groupName, "kColliderSize_", kColliderSize_);
	globalVariables->AddItem(groupName, "kInitialPosition_", kInitialPosition_);

	ApplyGlobalVariables();

#pragma endregion

	Setting();

}

void BossEnemy::Update()
{

#ifdef _DEBUG
	ApplyGlobalVariables();
#endif // _DEBUG

	for (TimedCall* timedCall : timedCalls_) {
		timedCall->Update();
	}

	worldTransform_.UpdateMatrix();

	Vector2 position = { worldTransform_.matWorld_.m[3][0],worldTransform_.matWorld_.m[3][1] };
	collider_.Update(position);

}

void BossEnemy::Draw(const ViewProjection& viewProjection)
{

	model_->Draw(worldTransform_, viewProjection);

}

void BossEnemy::Setting()
{

	// ワールドトランスフォーム
	worldTransform_.translation_ = kInitialPosition_;
	worldTransform_.rotation_ = { 0.0f,0.0f,0.0f };
	worldTransform_.scale_ = { 1.0f,1.0f,1.0f };
	worldTransform_.UpdateMatrix();

	// コライダー
	collider_.Initialize(&worldTransform_, kColliderSize_);
	collider_.SetCollisionAttribute(CollisionAttribute::bossEnemy);
	uint32_t mask = 0xffffffff;
	mask -= CollisionAttribute::bossEnemy + CollisionAttribute::blockEnemyAttack + CollisionAttribute::blockScaffold + CollisionAttribute::blockScaffoldColor;
	collider_.SetCollisionMask(mask);

	// HP
	hp_ = kInitialHp_;

	//timedCallの解放
	timedCalls_.remove_if([](TimedCall* timedCall) {
		delete timedCall;
		return true;
	});

	// 死亡フラグ
	isDead_ = false;

	// 仮置き
	SetDropBlockTimer();

}

void BossEnemy::OnCollision(uint32_t collisonObj, WorldTransform* worldTransform)
{
	if (collisonObj & CollisionAttribute::blockPlayerAttack) {
		Damage();
		effectManager_->SetIsStop(true);
		effectManager_->SetIsShake(true);
		Vector3 pos = { worldTransform->matWorld_.m[3][0],worldTransform->matWorld_.m[3][1], worldTransform->matWorld_.m[3][2] };
		particleManager_->RandomRespown(pos);
	}
	worldTransform;

}

void BossEnemy::DropBlock()
{

	enemyAttackFunc_();
	SetDropBlockTimer();

}

void BossEnemy::SetDropBlockTimer()
{
	timedCalls_.push_back(new TimedCall(std::bind(&BossEnemy::DropBlock, this), kDropBlockInterval_));
}

void BossEnemy::Damage()
{

	hp_ -=  blockManager_->GetBossDamage();
	if (hp_ <= 0) {
		hp_ = 0;
		isDead_ = false;
	}

}

void BossEnemy::ApplyGlobalVariables()
{

	// 調整項目クラスのインスタンス取得
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	// グループ名の設定
	const char* groupName = "BossEnemy";

	kDropBlockInterval_ = globalVariables->GetIntValue(groupName, "kDropBlockInterval_");
	kInitialHp_ = static_cast<uint32_t>(globalVariables->GetIntValue(groupName, "kInitialHp_"));
	kColliderSize_ = globalVariables->GetVector2Value(groupName, "kColliderSize_");
	kInitialPosition_ = globalVariables->GetVector3Value(groupName, "kInitialPosition_");

}

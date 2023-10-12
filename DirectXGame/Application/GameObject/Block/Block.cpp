#include "Block.h"
#include "Application/GameObject/BlockManager/BlockManager.h"
#include "Application/GameObject/PlayerAttack/PlayerAttack.h"
#include "Application/GameObject/Area/Area.h"
#include <Application/Others/Math2d/Math2d.h>

Block::~Block()
{

	if (state_) {
		delete state_;
	}

}

void Block::Initialize(Model* model, BlockState blockState, const Vector3& transfrom, const Vector2& colliderSize, BlockManager* blockManager)
{

	// ワールドトランスフォーム
	worldTransform_.Initialize();
	worldTransform_.translation_ = transfrom;
	worldTransform_.rotation_ = { 0.0f,0.0f,0.0f };
	worldTransform_.scale_ = { 1.0f,1.0f,1.0f };
	worldTransform_.UpdateMatrix();

	// モデル
	model_ = model;

	// 速度
	velocity_ = {0.0f,0.0f};

	// コライダーサイズ
	Vector2 position = {worldTransform_.matWorld_.m[3][0],worldTransform_.matWorld_.m[3][1] };
	collider_.Initialize(&worldTransform_, colliderSize);

	// 状態名
	stateName_ = blockState;

	// ブロックマネージャー
	blockManager_ = blockManager;

	uint32_t mask = 0xffffffff;

	// 状態
	switch (stateName_)
	{
	case kScaffold:
		state_ = new BlockStateScaffold();
		collider_.SetCollisionAttribute(CollisionAttribute::blockScaffold);
		mask -= CollisionAttribute::blockScaffold + CollisionAttribute::blockScaffoldColor + CollisionAttribute::blockPlayerAttack;
		collider_.SetCollisionMask(mask);
		// テクスチャハンドル
		textureHandle_ = blockManager_->GetTextureHandles().at(BlockState::kScaffold);
		break;

	case kEnemyAttack:
		state_ = new BlockStateEnemyAttack();
		collider_.SetCollisionAttribute(CollisionAttribute::blockEnemyAttack);
		mask -= CollisionAttribute::blockEnemyAttack + CollisionAttribute::bossEnemy;
		collider_.SetCollisionMask(mask);
		// テクスチャハンドル
		textureHandle_ = blockManager_->GetTextureHandles().at(BlockState::kEnemyAttack);
		break;

	default:
		break;
	}

	state_->Initialize(this);
	collider_.SetFunction(collisionFunction_);

	// 死亡フラグ
	isDead_ = false;

	// 親か
	isParent_ = true;

}

void Block::Update()
{

	//行動
	state_->Update();

	worldTransform_.UpdateMatrix();
	
	Vector2 position = { worldTransform_.matWorld_.m[3][0],worldTransform_.matWorld_.m[3][1] };
	collider_.Update(position);

}

void Block::Draw(const ViewProjection& viewProjection)
{

	model_->Draw(worldTransform_, viewProjection, textureHandle_);

}

void Block::ChangeState(BlockState blockstate)
{

	uint32_t mask = 0xffffffff;

	if (state_) {
		delete state_;
		stateName_ = blockstate;
		switch (stateName_)
		{
		case kScaffold:
			state_ = new BlockStateScaffold();
			collider_.SetCollisionAttribute(CollisionAttribute::blockScaffold);
			mask -= CollisionAttribute::blockScaffold + CollisionAttribute::blockScaffoldColor + CollisionAttribute::blockPlayerAttack;
			collider_.SetCollisionMask(mask);
			// テクスチャハンドル
			textureHandle_ = blockManager_->GetTextureHandles().at(BlockState::kScaffold);
			break;

		case kScaffoldColor:
			state_ = new BlockStateScaffoldColor();
			collider_.SetCollisionAttribute(CollisionAttribute::blockScaffoldColor);
			mask -= CollisionAttribute::blockScaffold + CollisionAttribute::blockScaffoldColor + CollisionAttribute::blockPlayerAttack;
			collider_.SetCollisionMask(mask);
			// テクスチャハンドル
			textureHandle_ = blockManager_->GetTextureHandles().at(BlockState::kScaffoldColor);
			break;

		case kPlayerAttack:
			state_ = new BlockStatePlayerAttack();
			collider_.SetCollisionAttribute(CollisionAttribute::blockPlayerAttack);
			mask -= CollisionAttribute::blockScaffold + CollisionAttribute::blockScaffoldColor;
			collider_.SetCollisionMask(mask);
			// テクスチャハンドル
			textureHandle_ = blockManager_->GetTextureHandles().at(BlockState::kPlayerAttack);
			break;

		case kEnemyAttack:
			state_ = new BlockStateEnemyAttack();
			mask -= CollisionAttribute::blockEnemyAttack + CollisionAttribute::bossEnemy;
			collider_.SetCollisionAttribute(CollisionAttribute::blockEnemyAttack);
			collider_.SetCollisionMask(mask);
			// テクスチャハンドル
			textureHandle_ = blockManager_->GetTextureHandles().at(BlockState::kEnemyAttack);
			break;

		default:
			break;
		}

		state_->Initialize(this);
		collider_.SetFunction(collisionFunction_);
	}

}

void Block::ScaffoldRise()
{

	// 足場以外はreturn
	if (stateName_ == kPlayerAttack || stateName_ == kEnemyAttack) {
		return;
	}

	worldTransform_.translation_.y += collider_.GetSize().y;
	worldTransform_.UpdateMatrix();

}

void Block::ParentChange(WorldTransform* parent)
{

	worldTransform_.translation_.x = worldTransform_.matWorld_.m[3][0] - parent->matWorld_.m[3][0];
	worldTransform_.translation_.y = worldTransform_.matWorld_.m[3][1] - parent->matWorld_.m[3][1];
	worldTransform_.parent_ = parent;
	worldTransform_.UpdateMatrix();

}

void Block::OnCollision(uint32_t collisonObj, WorldTransform* worldTransform)
{

	collisionFunction_(collisonObj, worldTransform);

}

void BlockStateScaffold::Initialize(Block* pBlock)
{

	// ポインタ
	pBlock_ = pBlock;

	//コールバック設定
	std::function<void(uint32_t, WorldTransform*)> f = std::function<void(uint32_t, WorldTransform*)>(std::bind(&BlockStateScaffold::OnCollision, this, std::placeholders::_1, std::placeholders::_2));
	pBlock_->SetCollisionFunction(f);

}

void BlockStateScaffold::Update()
{
}

void BlockStateScaffold::OnCollision(uint32_t collisonObj, WorldTransform* worldTransform)
{

	//ステートを色足場に変更
	if (collisonObj & CollisionAttribute::player) {
		
		//上面と当たったか

		Vector2 playerPos = { worldTransform->matWorld_.m[3][0],worldTransform->matWorld_.m[3][1] };
		Vector2 blockPos = { pBlock_->GetWorldTransform().matWorld_.m[3][0], pBlock_->GetWorldTransform().matWorld_.m[3][1]};

		Vector2 blockLT = { blockPos.x - pBlock_->GetCollider().GetSize().x / 2.0f, blockPos.y + pBlock_->GetCollider().GetSize().y / 2.0f };
		Vector2 blockRT = { blockPos.x + pBlock_->GetCollider().GetSize().x / 2.0f, blockPos.y + pBlock_->GetCollider().GetSize().y / 2.0f };
		if (Math2d::segmentsCrossing(playerPos, blockPos, blockLT, blockRT)) {
			pBlock_->ChangeState(BlockState::kScaffoldColor);		
		}
	}

}

void BlockStateScaffoldColor::Initialize(Block* pBlock)
{

	// ポインタ
	pBlock_ = pBlock;

	//コールバック設定
	std::function<void(uint32_t, WorldTransform*)> f = std::function<void(uint32_t, WorldTransform*)>(std::bind(&BlockStateScaffoldColor::OnCollision, this, std::placeholders::_1, std::placeholders::_2));
	pBlock_->SetCollisionFunction(f);

}

void BlockStateScaffoldColor::Update()
{
}

void BlockStateScaffoldColor::OnCollision(uint32_t collisonObj, WorldTransform* worldTransform)
{

	if (collisonObj & CollisionAttribute::blockEnemyAttack) {

		Vector2 partnerPos = { worldTransform->matWorld_.m[3][0],worldTransform->matWorld_.m[3][1] };
		Vector2 blockPos = { pBlock_->GetWorldTransform().matWorld_.m[3][0], pBlock_->GetWorldTransform().matWorld_.m[3][1] };

		Vector2 blockLT = { blockPos.x - pBlock_->GetCollider().GetSize().x / 2.0f, blockPos.y + pBlock_->GetCollider().GetSize().y / 2.0f };
		Vector2 blockRT = { blockPos.x + pBlock_->GetCollider().GetSize().x / 2.0f, blockPos.y + pBlock_->GetCollider().GetSize().y / 2.0f };
		Vector2 blockLB = { blockPos.x - pBlock_->GetCollider().GetSize().x / 2.0f, blockPos.y - pBlock_->GetCollider().GetSize().y / 2.0f };
		Vector2 blockRB = { blockPos.x + pBlock_->GetCollider().GetSize().x / 2.0f, blockPos.y - pBlock_->GetCollider().GetSize().y / 2.0f };

		if (Math2d::segmentsCrossing(partnerPos, blockPos, blockLT, blockLB) || Math2d::segmentsCrossing(partnerPos, blockPos, blockRT, blockRB)) {
			return;
		}

		if (Math2d::segmentsCrossing(partnerPos, blockPos, blockLT, blockRT) || Math2d::segmentsCrossing(partnerPos, blockPos, blockLB, blockRB)) {
			pBlock_->ChangeState(BlockState::kScaffold);
		}

	}

}

void BlockStatePlayerAttack::Initialize(Block* pBlock)
{

	// ポインタ
	pBlock_ = pBlock;

	//コールバック設定
	std::function<void(uint32_t, WorldTransform*)> f = std::function<void(uint32_t, WorldTransform*)>(std::bind(&BlockStatePlayerAttack::OnCollision, this, std::placeholders::_1, std::placeholders::_2));
	pBlock_->SetCollisionFunction(f);

}

void BlockStatePlayerAttack::Update()
{

	WorldTransform worldTransform = pBlock_->GetWorldTransform();
	worldTransform.translation_.y += pBlock_->GetVelocity().y;
	pBlock_->SetWorldTransform(worldTransform);

	if (worldTransform.matWorld_.m[3][1] >= pBlock_->GetBlockManager()->GetArea()->kBlockDeadLine_) {
		pBlock_->SetIsDead(true);
		pBlock_->GetBlockManager()->SetFireBlockCount(pBlock_->GetBlockManager()->GetFireBlockCount() - 1);
	}

}

void BlockStatePlayerAttack::OnCollision(uint32_t collisonObj, WorldTransform* worldTransform)
{
	
	//合体
	if (collisonObj & CollisionAttribute::blockPlayerAttack){

		Vector2 partnerPos = { worldTransform->matWorld_.m[3][0],worldTransform->matWorld_.m[3][1] };
		Vector2 blockPos = { pBlock_->GetWorldTransform().matWorld_.m[3][0], pBlock_->GetWorldTransform().matWorld_.m[3][1]};

		Vector2 blockLT = { blockPos.x - pBlock_->GetCollider().GetSize().x / 2.0f, blockPos.y + pBlock_->GetCollider().GetSize().y / 2.0f };
		Vector2 blockRT = { blockPos.x + pBlock_->GetCollider().GetSize().x / 2.0f, blockPos.y + pBlock_->GetCollider().GetSize().y / 2.0f };
		Vector2 blockLB = { blockPos.x - pBlock_->GetCollider().GetSize().x / 2.0f, blockPos.y - pBlock_->GetCollider().GetSize().y / 2.0f };
		Vector2 blockRB = { blockPos.x + pBlock_->GetCollider().GetSize().x / 2.0f, blockPos.y - pBlock_->GetCollider().GetSize().y / 2.0f };
		if (Math2d::segmentsCrossing(partnerPos, blockPos, blockLT, blockLB) || Math2d::segmentsCrossing(partnerPos, blockPos, blockRT, blockRB)) {
			return;
		}

		if (Math2d::segmentsCrossing(partnerPos, blockPos, blockLT, blockRT) || Math2d::segmentsCrossing(partnerPos, blockPos, blockLB, blockRB)) {
			
			// 合体用保存データにいれる
			if (!pBlock_->GetBlockManager()->GetPlayerAttackUnionData()) {
				pBlock_->GetBlockManager()->SetPlayerAttackUnionData(pBlock_->GetPlayerAttack());
			}
			// 合体開始　今回のデータ
			else {
				pBlock_->GetBlockManager()->PlayerAttackUnion(pBlock_->GetPlayerAttack());
			}

		}
	}
	else if (collisonObj & CollisionAttribute::bossEnemy) {
		pBlock_->SetIsDead(true);
		pBlock_->GetPlayerAttack()->ComboUp();
		pBlock_->GetBlockManager()->SetBossDamage(pBlock_->GetPlayerAttack()->GetCombo());
	}

}

void BlockStateEnemyAttack::Initialize(Block* pBlock)
{
	// ポインタ
	pBlock_ = pBlock;

	//コールバック設定
	std::function<void(uint32_t, WorldTransform*)> f = std::function<void(uint32_t, WorldTransform*)>(std::bind(&BlockStateEnemyAttack::OnCollision, this, std::placeholders::_1, std::placeholders::_2));
	pBlock_->SetCollisionFunction(f);

}

void BlockStateEnemyAttack::Update()
{
	WorldTransform worldTransform = pBlock_->GetWorldTransform();
	worldTransform.translation_.y -= pBlock_->GetVelocity().y;
	pBlock_->SetWorldTransform(worldTransform);

	if (worldTransform.matWorld_.m[3][1] <= pBlock_->GetBlockManager()->GetArea()->kPositionMin_.y + pBlock_->GetCollider().GetSize().y / 2.0f) {
		worldTransform.translation_.y = pBlock_->GetBlockManager()->GetArea()->kPositionMin_.y + pBlock_->GetCollider().GetSize().y / 2.0f;
		pBlock_->SetWorldTransform(worldTransform);
		pBlock_->ChangeState(BlockState::kScaffold);
	}

	// 警告を表示する
	pBlock_->GetBlockManager()->Warning(worldTransform.matWorld_.m[3][0]);

}

void BlockStateEnemyAttack::OnCollision(uint32_t collisonObj, WorldTransform* worldTransform)
{

	Vector2 partnerPos = { worldTransform->matWorld_.m[3][0],worldTransform->matWorld_.m[3][1] };
	Vector2 blockPos = { pBlock_->GetWorldTransform().matWorld_.m[3][0], pBlock_->GetWorldTransform().matWorld_.m[3][1] };

	Vector2 blockLT = { blockPos.x - pBlock_->GetCollider().GetSize().x / 2.0f, blockPos.y + pBlock_->GetCollider().GetSize().y / 2.0f };
	Vector2 blockRT = { blockPos.x + pBlock_->GetCollider().GetSize().x / 2.0f, blockPos.y + pBlock_->GetCollider().GetSize().y / 2.0f };
	Vector2 blockLB = { blockPos.x - pBlock_->GetCollider().GetSize().x / 2.0f, blockPos.y - pBlock_->GetCollider().GetSize().y / 2.0f };
	Vector2 blockRB = { blockPos.x + pBlock_->GetCollider().GetSize().x / 2.0f, blockPos.y - pBlock_->GetCollider().GetSize().y / 2.0f };

	WorldTransform wT;
	wT.Initialize();
	wT.translation_.x = partnerPos.x;
	wT.translation_.y = partnerPos.y + pBlock_->GetCollider().GetSize().y;
	wT.UpdateMatrix();

	if (collisonObj & CollisionAttribute::blockPlayerAttack) {

		if (Math2d::segmentsCrossing(partnerPos, blockPos, blockLT, blockLB) || Math2d::segmentsCrossing(partnerPos, blockPos, blockRT, blockRB)) {
			return;
		}

		if (Math2d::segmentsCrossing(partnerPos, blockPos, blockLT, blockRT) || Math2d::segmentsCrossing(partnerPos, blockPos, blockLB, blockRB)) {
			pBlock_->SetWorldTransform(wT);
			pBlock_->GetBlockManager()->EnemyAttackPlayerAttackChange(pBlock_);
			pBlock_->ChangeState(BlockState::kPlayerAttack);
		}

	}
	else if (collisonObj & CollisionAttribute::blockScaffold || collisonObj & CollisionAttribute::blockScaffoldColor) {

		if (Math2d::segmentsCrossing(partnerPos, blockPos, blockLT, blockLB) || Math2d::segmentsCrossing(partnerPos, blockPos, blockRT, blockRB)) {
			return;
		}

		if (Math2d::segmentsCrossing(partnerPos, blockPos, blockLT, blockRT) || Math2d::segmentsCrossing(partnerPos, blockPos, blockLB, blockRB)) {
			pBlock_->SetWorldTransform(wT);
			pBlock_->ChangeState(BlockState::kScaffold);
		}

	}
	else if (collisonObj & CollisionAttribute::player) {

		pBlock_->SetIsDead(true);

	}

}

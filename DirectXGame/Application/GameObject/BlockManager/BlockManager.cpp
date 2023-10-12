#include "BlockManager.h"

#include "Application/GameObject/Area/Area.h"
#include <GlobalVariables.h>

BlockManager::~BlockManager()
{

	//block_の解放
	for (Block* block : blocks_) {
		delete block;
	}
	//timedCall_の解放
	for (TimedCall* timedCall : timedCalls_) {
		delete timedCall;
	}
	//playerAttacks_の解放
	for (PlayerAttack* playerAttack : playerAttacks_) {
		delete playerAttack;
	}

}

void BlockManager::Initialize(Model* model, std::vector<uint32_t> textureHandles, Model* warningModel)
{

	// モデル
	model_ = model;

	// テクスチャハンドル
	textureHandles_ = textureHandles;

	// 警告モデル
	warningModel_ = warningModel;

#pragma region 調整項目クラス
	// 調整項目クラスのインスタンス取得
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	// グループ名設定
	const char* groupName = "Block";
	// 指定した名前でグループ追加
	globalVariables->CreateGroup(groupName);

	// メンバ変数の調整したい項目をグローバル変数に追加

	globalVariables->AddItem(groupName, "kBaseFireBlockSpeed_", kBaseFireBlockSpeed_);
	globalVariables->AddItem(groupName, "kBaseScaffoldBlockGenerateInterval_", kBaseScaffoldBlockGenerateInterval_);
	globalVariables->AddItem(groupName, "kColliderSize_", kColliderSize_);

	ApplyGlobalVariables();

#pragma endregion

	Setting();

}

void BlockManager::Update()
{

#ifdef _DEBUG
	ApplyGlobalVariables();
#endif // _DEBUG

	// 警告を表示するか
	showWarning = false;

	for (Block* block : blocks_) {
		block->Update();
	}

	for (TimedCall* timedCall : timedCalls_) {
		timedCall->Update();
	}

	for (PlayerAttack* playerAttack : playerAttacks_) {
		playerAttack->Update();
	}

	// 保存データをNULLに 
	playerAttackUnionData_ = nullptr;

	CheckHeight();

}

void BlockManager::Draw(const ViewProjection& viewProjection)
{
	for (Block* block : blocks_) {
		block->Draw(viewProjection);
	}

	if (showWarning) {
		warningModel_->Draw(warningWorldTransform_, viewProjection);
	}

}

void BlockManager::Setting()
{

	//blockの解放
	blocks_.remove_if([](Block* block) {
		delete block;
		return true;
	});

	//timedCallの解放
	timedCalls_.remove_if([](TimedCall* timedCall) {
		delete timedCall;
		return true;
	});

	//playerAttackの解放
	playerAttacks_.remove_if([](PlayerAttack* playerAttack) {
		delete playerAttack;
		return true;
	});

	//足場ブロック生成インターバル
	scaffoldBlockGenerateInterval_ = kBaseScaffoldBlockGenerateInterval_;

	// 発射されているブロック数
	fireBlockCount_ = 0;

	// ゲームオーバーフラグ
	gameOver_ = false;

	// 警告ワールドトランスフォーム
	warningWorldTransform_.Initialize();

	// 警告を表示するか
	showWarning = false;

	SetScaffoldBlockGenerateTimer();

}

void BlockManager::DeleteBlock()
{

	//block_の解放
	blocks_.remove_if([](Block* block) {
		if (block->IsDead()) {
			delete block;
			return true;
		}
		return false;
	});

	//playerAttacks_の解放
	playerAttacks_.remove_if([](PlayerAttack* playerAttack){
		if (playerAttack->IsDead()) {
			delete playerAttack;
			return true;
		}
		return false;
	});

}

void BlockManager::BlockFiring()
{

	std::vector<Block*> fireBlocks;

	//とぶか?
	for (Block* block : blocks_) {
		if (block->GetStateName() == BlockState::kScaffoldColor) {
			fireBlockCount_++;
			fireBlocks.push_back(block);
		}
	}

	// プレイヤーのアタック関数
	std::list<PlayerAttack*> playerAttacks;

	//発射処理
	for (Block* block : fireBlocks) {
		block->ChangeState(BlockState::kPlayerAttack);

		// 所属したか
		bool isJoin = false;
		for (PlayerAttack* playerAttack : playerAttacks) {
			if (playerAttack->GetParentBlock()->GetWorldTransform().translation_.y
				== block->GetWorldTransform().translation_.y) {
				playerAttack->AddBlockList(block);
				Vector2 speed = { 0.0f, playerAttack->GetParentBlock()->GetVelocity().y + kBaseFireBlockSpeed_ };
				playerAttack->GetParentBlock()->SetVelocity(speed);
				isJoin = true;
				break;
			}
		}

		// してない(新しいアタックオブジェクト生成)
		if (!isJoin) {
			PlayerAttack* playerAttack = new PlayerAttack();
			playerAttack->Initialize(block);
			playerAttack->SetParentBlock(block);
			Vector2 speed = { 0.0f , kBaseFireBlockSpeed_ };
			block->SetVelocity(speed);
			playerAttacks.push_back(playerAttack);
		}
	}

	for (PlayerAttack* playerAttack : playerAttacks) {
		playerAttacks_.push_back(playerAttack);
	}

}

void BlockManager::AmazingBlockFiring(float posY)
{

	std::vector<Block*> fireBlocks;

	//とぶか?
	for (Block* block : blocks_) {
		if ( (block->GetStateName() == BlockState::kScaffold || block->GetStateName() == BlockState::kScaffoldColor) && 
			block->GetWorldTransform().matWorld_.m[3][1] >= posY) {
			fireBlockCount_++;
			fireBlocks.push_back(block);
		}
	}

	// プレイヤーのアタック関数
	std::list<PlayerAttack*> playerAttacks;

	//発射処理
	for (Block* block : fireBlocks) {
		block->ChangeState(BlockState::kPlayerAttack);
		PlayerAttack* playerAttack = new PlayerAttack();
		playerAttack->Initialize(block);
		playerAttack->SetParentBlock(block);
		Vector2 speed = { 0.0f , kBaseAmazingFireBlockSpeed_ };
		block->SetVelocity(speed);
		playerAttacks_.push_back(playerAttack);
	}

}

void BlockManager::PlayerAttackUnion(PlayerAttack* playerAttackUnionData)
{

	if (playerAttackUnionData == playerAttackUnionData_) {
		return;
	}

	// どちらの親が下にあるか
	if (playerAttackUnionData->GetParentBlock()->GetWorldTransform().translation_.y >
		playerAttackUnionData_->GetParentBlock()->GetWorldTransform().translation_.y) {

		//速度
		if (kBaseAmazingFireBlockSpeed_ == playerAttackUnionData_->GetParentBlock()->GetVelocity().y ||
			kBaseAmazingFireBlockSpeed_ == playerAttackUnionData->GetParentBlock()->GetVelocity().y) {
			playerAttackUnionData_->GetParentBlock()->SetVelocity(Vector2(0.0f, kBaseAmazingFireBlockSpeed_));
			playerAttackUnionData->GetParentBlock()->SetVelocity(Vector2(0.0f, 0.0f));
		}
		else {
			playerAttackUnionData_->GetParentBlock()->SetVelocity(playerAttackUnionData->GetParentBlock()->GetVelocity());
			playerAttackUnionData->GetParentBlock()->SetVelocity(Vector2(0.0f, 0.0f));
		}
		// 合体
		playerAttackUnionData_->AddBlockList(playerAttackUnionData->GetBlock());
		WorldTransform* a = playerAttackUnionData_->GetParentBlock()->GetWorldTransformAddress();
		playerAttackUnionData->GetParentBlock()->ParentChange(a);


		// 合体したPlayerAttackを死亡判定に
		playerAttackUnionData->SetIsDead(true);

	}
	else{

		//速度
		if (kBaseAmazingFireBlockSpeed_ == playerAttackUnionData->GetParentBlock()->GetVelocity().y ||
			kBaseAmazingFireBlockSpeed_ == playerAttackUnionData_->GetParentBlock()->GetVelocity().y) {
			playerAttackUnionData->GetParentBlock()->SetVelocity(Vector2(0.0f, kBaseAmazingFireBlockSpeed_));
			playerAttackUnionData_->GetParentBlock()->SetVelocity(Vector2(0.0f, 0.0f));
		}
		// 合体
		playerAttackUnionData->AddBlockList(playerAttackUnionData_->GetBlock());
		WorldTransform* a = playerAttackUnionData->GetParentBlock()->GetWorldTransformAddress();
		playerAttackUnionData_->GetParentBlock()->ParentChange(a);

		// 合体したPlayerAttackを死亡判定に
		playerAttackUnionData_->SetIsDead(true);

	}

	// 保存データをNULLに 
	playerAttackUnionData_ = nullptr;

}

void BlockManager::EnemyAttackBlockGenerate()
{

	// ブロック新規作成
	Block* block = new Block();

	// 低い場所探し
	size_t min = 0;
	for (size_t i = 1; i < sizeof(height) / sizeof(float); i++)
	{
		if (height[i] < height[min]) {
			min = i;
		}
	}
	// 位置
	Vector3 transform =
	{ kColliderSize_.x * min + kColliderSize_.x / 2.0f,
		(static_cast<size_t>(area_->kMap_.y) - 1) * kColliderSize_.y + kColliderSize_.y / 2.0f,
		0.0f };
	// 初期化
	block->Initialize(model_, BlockState::kEnemyAttack, transform, kColliderSize_, this);
	// 速度設定
	block->SetVelocity(Vector2( 0.0f, kBaseFallBlockSpeed_));
	// プッシュバック
	blocks_.push_back(block);

}

void BlockManager::EnemyAttackPlayerAttackChange(Block* block)
{
	PlayerAttack* playerAttack = new PlayerAttack();
	playerAttack->Initialize(block);
	playerAttack->SetParentBlock(block);
	Vector2 speed = { 0.0f , kBaseFireBlockSpeed_ };
	block->SetVelocity(speed);
	playerAttacks_.push_back(playerAttack);

}

void BlockManager::Warning(float posX)
{

	int x = static_cast<int>((posX - kColliderSize_.x / 2.0f) / kColliderSize_.x);

	warningWorldTransform_.translation_ = { posX, height[x], -1.0f };
	warningWorldTransform_.UpdateMatrix();

	showWarning = true;

}

void BlockManager::ScaffoldBlockGenerate()
{
	for (Block* block : blocks_) {
		block->ScaffoldRise();
	}

	for (size_t i = 0; i < static_cast<size_t>(area_->kMap_.x); i++) {
		Block* block = new Block();
		Vector3 transform =
		{ kColliderSize_.x / 2.0f + kColliderSize_.x * static_cast<float>(i),
			kColliderSize_.y / 2.0f, 0.0f };
		block->Initialize(model_, BlockState::kScaffold, transform, kColliderSize_, this);
		blocks_.push_back(block);
	}

	SetScaffoldBlockGenerateTimer();

}

void BlockManager::SetScaffoldBlockGenerateTimer()
{
	timedCalls_.push_back(new TimedCall(std::bind(&BlockManager::ScaffoldBlockGenerate, this), scaffoldBlockGenerateInterval_));
}

void BlockManager::CheckHeight()
{

	for (size_t i = 0; i < sizeof(height) / sizeof(float); i++)
	{
		height[i] = kColliderSize_.y / 2.0f;
	}

	for (Block* block : blocks_)
	{
		if (block->GetStateName() == BlockState::kScaffold || block->GetStateName() == BlockState::kScaffoldColor) {
			size_t x = static_cast<size_t>((block->GetWorldTransform().matWorld_.m[3][0] - kColliderSize_.x / 2.0f) / kColliderSize_.x);
			height[x] += kColliderSize_.y;
			
			// ゲームオーバーか
			if (height[x] >= area_->kYellowLine_) {
				gameOver_ = true;
			}
		}

	}

}

void BlockManager::ApplyGlobalVariables()
{

	// 調整項目クラスのインスタンス取得
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	// グループ名の設定
	const char* groupName = "Block";

	kBaseFireBlockSpeed_ = globalVariables->GetFloatValue(groupName, "kBaseFireBlockSpeed_");
	kBaseScaffoldBlockGenerateInterval_ = globalVariables->GetIntValue(groupName, "kBaseScaffoldBlockGenerateInterval_");
	kColliderSize_ = globalVariables->GetVector2Value(groupName, "kColliderSize_");

}

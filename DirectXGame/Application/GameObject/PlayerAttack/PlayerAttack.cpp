#include "PlayerAttack.h"

void PlayerAttack::Initialize(Block* block)
{

	// ブロック
	blocks_.push_back(block);

	// 親ブロック
	parentBlock_ = blocks_.front();

	// コンボ
	combo_ = 0;

	// このフレームでコンボが増えたか
	isComboUp_ = false;

	// 死んでいる(役割的に)
	isDead_ = false;

	block->SetPlayerAttack(this);

}

void PlayerAttack::Update()
{

	DeleteBlock();

	isComboUp_ = false;

	if (parentBlock_ == nullptr) {
		isDead_ = true;
	}

}

void PlayerAttack::DeleteBlock()
{

	//blockの解放
	blocks_.remove_if([](Block* block) {
		if (!block) {
			return true;
		}
		return false;
	});

}

void PlayerAttack::ComboUp()
{
	if (isComboUp_) {
		return;
	}

	combo_++;

	isComboUp_ = true;

}

void PlayerAttack::AddBlockList(Block* block)
{

	blocks_.push_back(block);
	WorldTransform worldTransform;
	worldTransform.Initialize();
	worldTransform.parent_ = parentBlock_->GetWorldTransformAddress();
	worldTransform.translation_.x = block->GetWorldTransform().matWorld_.m[3][0] - parentBlock_->GetWorldTransform().matWorld_.m[3][0];
	worldTransform.UpdateMatrix();
	block->SetWorldTransform(worldTransform);
	block->SetPlayerAttack(this);

}

void PlayerAttack::AddBlockList(std::list<Block*> blocks)
{

	for (Block* block : blocks) {
		blocks_.push_back(block);
		block->SetPlayerAttack(this);
	}

}

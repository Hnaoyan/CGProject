#pragma once
#include "Application/GameObject/Block/Block.h"
#include <list>

class PlayerAttack
{

public: // メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Block* block);
	
	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// ブロックリストから削除
	/// </summary>
	void DeleteBlock();

	/// <summary>
	/// コンボアップ
	/// </summary>
	void ComboUp();

	/// <summary>
	/// ブロックリスト追加
	/// </summary>
	/// <param name="blocks"></param>
	void AddBlockList(Block* block);

	/// <summary>
	/// ブロックリスト追加
	/// </summary>
	/// <param name="blocks"></param>
	void AddBlockList(std::list<Block*> blocks);

public: //アクセッサ

	/// <summary>
	/// 親ブロックゲッター
	/// </summary>
	/// <returns></returns>
	Block* GetParentBlock() { return parentBlock_; }

	/// <summary>
	/// 親ブロックセッター
	/// </summary>
	/// <returns></returns>
	void SetParentBlock(Block* parentBlock) { parentBlock_ = parentBlock; }

	/// <summary>
	/// 死んでるか
	/// </summary>
	/// <returns></returns>
	bool IsDead() { return isDead_; }

	/// <summary>
	/// 死んでるかセッター
	/// </summary>
	/// <param name="isDead"></param>
	void SetIsDead(bool isDead) { isDead_ = isDead; }

	/// <summary>
	/// ブロックゲッター
	/// </summary>
	/// <returns></returns>
	std::list<Block*> GetBlock() { return blocks_; }

	/// <summary>
	/// コンボゲッター
	/// </summary>
	/// <returns></returns>
	uint32_t GetCombo() { return combo_; }

private: //メンバ変数

	// ブロック
	std::list<Block*> blocks_;

	// 親ブロック
	Block* parentBlock_;

	// コンボ
	uint32_t combo_;

	// このフレームでコンボが増えたか
	bool isComboUp_;

	// 死んでいる(役割的に)
	bool isDead_;

};


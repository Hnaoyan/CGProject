#pragma once
#include <list>
#include <vector>
#include "Application/GameObject/Block/Block.h"
#include "Application/Others/TimedCall/TimedCall.h"
#include "Application/GameObject/PlayerAttack/PlayerAttack.h"

//前方宣言
class Area;

/// <summary>
/// ブロックマネージャー
/// </summary>
class BlockManager
{

public: // メンバ関数

	/// <summary>
	/// デストラクタ
	/// </summary>
	~BlockManager();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model, std::vector<uint32_t> textureHandles, Model* warningModel);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(const ViewProjection& viewProjection);

	/// <summary>
	/// 設定
	/// </summary>
	void Setting();

	/// <summary>
	/// ブロック削除
	/// </summary>
	void DeleteBlock();

	/// <summary>
	/// ブロック発射
	/// </summary>
	void BlockFiring();


	/// <summary>
	/// ブロック発射(大技)
	/// </summary>
	void AmazingBlockFiring(float posY);

	/// <summary>
	/// プレイヤーのアタック合体
	/// </summary>
	/// <param name="playerAttackUnionData"></param>
	/// <param name="match"></param>
	void PlayerAttackUnion(PlayerAttack* playerAttackUnionData);

	/// <summary>
	/// エネミーの攻撃ブロック生成
	/// </summary>
	void EnemyAttackBlockGenerate();

	/// <summary>
	/// エネミーの攻撃からプレイヤー攻撃へ
	/// </summary>
	void EnemyAttackPlayerAttackChange(Block* block);

	/// <summary>
	/// 警告を表示する準備
	/// </summary>
	/// <param name="posX"></param>
	void Warning(float posX);

private: // メンバ関数

	/// <summary>
	/// 足場ブロック生成
	/// </summary>
	void ScaffoldBlockGenerate();

	/// <summary>
	/// 足場ブロック生成セット
	/// </summary>
	void SetScaffoldBlockGenerateTimer();

	/// <summary>
	/// 高さ確認
	/// </summary>
	void CheckHeight();

	/// <summary>
	/// 調整項目適用関数
	/// </summary>
	void ApplyGlobalVariables();

public: // アクセッサ

	/// <summary>
	/// エリアゲッター
	/// </summary>
	/// <param name="area"></param>
	Area* GetArea() { return area_; }

	/// <summary>
	/// エリアセッター
	/// </summary>
	/// <param name="area"></param>
	void SetArea(Area* area) { area_ = area; }

	/// <summary>
	/// ブロックゲッター
	/// </summary>
	std::list<Block*> GetBlocks() { return blocks_; }

	/// <summary>
	/// テクスチャハンドルゲッター
	/// </summary>
	/// <returns></returns>
	std::vector<uint32_t> GetTextureHandles() { return textureHandles_; }

	/// <summary>
	/// コライダーサイズゲッター
	/// </summary>
	/// <returns></returns>
	Vector2 GetColliderSize() { return kColliderSize_; }

	/// <summary>
	/// 発射されているブロック数ゲッター
	/// </summary>
	/// <returns></returns>
	uint32_t GetFireBlockCount() { return fireBlockCount_; }

	/// <summary>
	/// 発射されているブロック数ゲッター
	/// </summary>
	/// <param name="fireBlockCount"></param>
	void SetFireBlockCount(uint32_t fireBlockCount) { fireBlockCount_ = fireBlockCount; }

	/// <summary>
	/// プレイヤーのアタック合体保存用ゲッター
	/// </summary>
	/// <returns></returns>
	PlayerAttack* GetPlayerAttackUnionData() { return playerAttackUnionData_; }

	/// <summary>
	/// プレイヤーのアタック合体保存用セッター
	/// </summary>
	/// <param name="playerAttackUnionData"></param>
	void SetPlayerAttackUnionData(PlayerAttack* playerAttackUnionData) { playerAttackUnionData_ = playerAttackUnionData; }

	/// <summary>
	/// ボスへのダメージ用ゲッター
	/// </summary>
	/// <returns></returns>
	uint32_t GetBossDamage() { return bossDamage_; }

	/// <summary>
	/// ボスへのダメージ用セッター
	/// </summary>
	/// <param name="bossDamage"></param>
	void SetBossDamage(uint32_t bossDamage) { bossDamage_ = bossDamage; }

	/// <summary>
	/// ゲームオーバーフラグゲッター
	/// </summary>
	/// <returns></returns>
	bool GetGameOver() { return gameOver_; }

private: // メンバ変数

	// ブロック
	std::list<Block*> blocks_;

	//モデル
	Model* model_ = nullptr;

	//時限発動のリスト
	std::list<TimedCall*> timedCalls_;

	// エリア
	Area* area_ = nullptr;

	// 足場ブロック生成インターバル
	int scaffoldBlockGenerateInterval_;

	// テクスチャハンドル
	std::vector<uint32_t> textureHandles_;

	// 発射されているブロック数
	uint32_t fireBlockCount_;

	// プレイヤーのアタック
	std::list<PlayerAttack*> playerAttacks_;

	// プレイヤーのアタック合体保存用
	PlayerAttack* playerAttackUnionData_ = nullptr;

	// ボスへのダメージ用
	uint32_t bossDamage_ = 0u;

	// それぞれの高さ
	float height[10];

	// ゲームオーバーフラグ
	bool gameOver_;

	// 警告モデル
	Model* warningModel_;

	// 警告ワールドトランスフォーム
	WorldTransform warningWorldTransform_;

	// 警告を表示するか
	bool showWarning;

private: // メンバ定数

	// 発射されているブロックベース速度
	float kBaseFireBlockSpeed_ = 0.05f;

	// 発射されているブロックベース速度Amazing
	float kBaseAmazingFireBlockSpeed_ = 0.8f;

	// 足場ブロック生成インターバルベース
	int kBaseScaffoldBlockGenerateInterval_ = 300;

	// コライダーサイズ
	Vector2 kColliderSize_ = { 2.0f, 2.0f};

	// 落下してくるブロックベース速度
	float kBaseFallBlockSpeed_ = 0.4f;

};


#pragma once

#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Model.h"

#include "Application/Others/RectangleCollider/RectangleCollider.h"

//前方宣言
class Block;
class BlockManager;
class PlayerAttack;

/// <summary>
/// ブロックの状態
/// </summary>
enum BlockState
{
	kScaffold, //足場
	kScaffoldColor, //色のついた足場
	kPlayerAttack, //プレイヤーの攻撃
	kEnemyAttack // エネミーの攻撃
};

/// <summary>
/// ブロックの状態
/// </summary>
class BaseBlockState
{

public: // メンバ関数
	
	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~BaseBlockState() {}

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="pBlock">ポインタ</param>
	virtual void Initialize(Block* pBlock) = 0;

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// 衝突を検出したら呼び出されるコールバック関数
	/// </summary>
	/// <param name="collisonObj">衝突したOBJ</param>
	virtual void OnCollision(uint32_t collisonObj, WorldTransform* worldTransform) = 0;

protected: // 継承メンバ変数

	//ポインタ
	Block* pBlock_ = nullptr;

};

/// <summary>
/// ブロックの状態(足場)
/// </summary>
class BlockStateScaffold : public BaseBlockState
{

public: // メンバ関数
	void Initialize(Block* pBlock) override;
	void Update()  override;
	void OnCollision(uint32_t collisonObj, WorldTransform* worldTransform) override;

};

/// <summary>
/// ブロックの状態(足場)
/// </summary>
class BlockStateScaffoldColor : public BaseBlockState
{

public: // メンバ関数
	void Initialize(Block* pBlock) override;
	void Update()  override;
	void OnCollision(uint32_t collisonObj, WorldTransform* worldTransform) override;

};

/// <summary>
/// ブロックの状態(プレイヤーの攻撃)
/// </summary>
class BlockStatePlayerAttack : public BaseBlockState
{

public: // メンバ関数
	void Initialize(Block* pBlock) override;
	void Update()  override;
	void OnCollision(uint32_t collisonObj, WorldTransform* worldTransform) override;

};

/// <summary>
/// ブロックの状態(エネミーの攻撃)
/// </summary>
class BlockStateEnemyAttack : public BaseBlockState
{

public: // メンバ関数
	void Initialize(Block* pBlock) override;
	void Update()  override;
	void OnCollision(uint32_t collisonObj, WorldTransform* worldTransform) override;

};

class Block
{

public: // メンバ関数

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Block();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	void Initialize(Model* model, BlockState blockState, const Vector3& transfrom, const Vector2& colliderSize, BlockManager* blockManager);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション</param>
	void Draw(const ViewProjection& viewProjection);

	/// <summary>
	/// 状態変更
	/// </summary>
	/// <param name="blockstate">新しい状態名</param>
	void ChangeState(BlockState blockstate);

	/// <summary>
	/// 足場上昇
	/// </summary>
	void ScaffoldRise();

	/// <summary>
	/// 親変更
	/// </summary>
	/// <param name="parent"></param>
	void ParentChange(WorldTransform* parent);

	/// <summary>
	/// 衝突を検出したら呼び出されるコールバック関数
	/// </summary>
	/// <param name="collisonObj">衝突したOBJ</param>
	void OnCollision(uint32_t collisonObj, WorldTransform* worldTransform);

public: // アクセッサ

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
	/// コライダーゲッター
	/// </summary>
	/// <returns></returns>
	RectangleCollider GetCollider() { return collider_; }

	/// <summary>
	/// コライダーアドレスゲッター
	/// </summary>
	/// <returns></returns>
	RectangleCollider* GetColliderAddress() { return &collider_; }

	/// <summary>
	/// 衝突コールバックセッター
	/// </summary>
	/// <param name="function">関数</param>
	void SetCollisionFunction(std::function<void(uint32_t, WorldTransform*)> collisionFunction) { collisionFunction_ = collisionFunction; }

	/// <summary>
	/// 衝突コールバックゲッター
	/// </summary>
	/// <param name="function">関数</param>
	std::function<void(uint32_t, WorldTransform*)> GetCollisionFunction() { return collisionFunction_; }

	/// <summary>
	/// 状態名ゲッター
	/// </summary>
	/// <returns></returns>
	BlockState GetStateName() { return stateName_; }

	/// <summary>
	/// ワールドトランスフォームゲッター
	/// </summary>
	/// <returns></returns>
	WorldTransform GetWorldTransform() { return worldTransform_; }

	/// <summary>
	/// ワールドトランスフォームゲッター
	/// </summary>
	/// <returns></returns>
	WorldTransform* GetWorldTransformAddress() { return &worldTransform_; }

	/// <summary>
	/// ワールドトランスフォームセッター
	/// </summary>
	/// <param name="worldTransform"></param>
	void SetWorldTransform(WorldTransform worldTransform) { worldTransform_ = worldTransform; }

	/// <summary>
	/// 速度ゲッター
	/// </summary>
	/// <returns></returns>
	Vector2 GetVelocity() { return velocity_; }

	/// <summary>
	/// 速度セッター
	/// </summary>
	/// <param name="velocity"></param>
	void SetVelocity(Vector2 velocity) { velocity_ = velocity; }

	/// <summary>
	/// ブロックマネージャーゲッター
	/// </summary>
	/// <returns></returns>
	BlockManager* GetBlockManager() { return blockManager_; }

	/// <summary>
	/// 自分の所属しているアタックゲッター
	/// </summary>
	/// <returns></returns>
	PlayerAttack* GetPlayerAttack() { return playerAttack_; }

	/// <summary>
	/// 自分の所属しているアタックセッター
	/// </summary>
	/// <param name="playerAttack"></param>
	void SetPlayerAttack(PlayerAttack* playerAttack) { playerAttack_ = playerAttack; }

private: // メンバ変数

	//ワールドトランスフォーム
	WorldTransform worldTransform_;

	// モデル
	Model* model_ = nullptr;

	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	// 速度
	Vector2 velocity_;

	//コライダー
	RectangleCollider collider_;

	// 状態
	BaseBlockState* state_;

	// 状態名
	BlockState stateName_;

	// 死亡フラグ
	bool isDead_;

	// 衝突コールバック
	std::function<void(uint32_t, WorldTransform*)> collisionFunction_;

	// ブロックマネージャー
	BlockManager* blockManager_ = nullptr;

	// 親か
	bool isParent_;

	// 自分の所属しているアタック
	PlayerAttack* playerAttack_ = nullptr;

};


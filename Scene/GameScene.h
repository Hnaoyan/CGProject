#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "FollowCamera.h"
#include "Player.h"
#include "Ground.h"
#include "Enemy.h"
#include <memory>
#include "BaseScene.h"
#include "Skydome.h"

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene : public BaseScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

public:
	void CheckAllCollision();

	void CheckCollisionPair() {};

	void CheckGroundToPlayer();

	enum StageNumber
	{
		kStageOne,
		kStageTwo,
		kStageThree,
		kStageClear,
		kNone,
	};

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	ViewProjection viewProjection_;

	int enemyCount_ = 0;

	uint32_t soundHandle_ = 0;

	int stageNum = kStageOne;

	int clearCount_ = 0;

	Sprite* clear_;

private:	// メンバポインタ
	// プレイヤー
	std::unique_ptr<Player> player_;
	std::unique_ptr<Model> plModel_;
	std::unique_ptr<Model> plBulletModel_;

	// エネミー
	std::unique_ptr<Model> eneModel_;

	std::list<Enemy*> enemys_;

	// カメラ
	//std::unique_ptr<DebugCamera> debugCamera_;
	std::unique_ptr<FollowCamera> followCamera_;

	bool isDebug_ = false;

	// 地面
	std::unique_ptr<Ground> ground_;
	std::unique_ptr<Model> groundModel_;
	std::unique_ptr<Skydome> skyDome_;
	std::unique_ptr<Model> skyDomeModel_;

	Sprite* info_;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};


#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

#include "CollisionManager.h"
#include "FollowCamera.h"
#include "BaseScene.h"
#include "BaseCamera.h"

#include "Player.h"
#include "Enemy.h"
#include "Goal.h"

#include "GroundManager.h"
#include "SkyDome.h"

#include <memory>

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

private:
	/// <summary>
	/// カメラ系の更新処理
	/// </summary>
	void CameraUpdate();

	void CheckCollision();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	ViewProjection viewProjection_;

private:
	std::unique_ptr<CollisionManager> colliderManager_;


private:	// メンバポインタ
	// カメラ
	//std::unique_ptr<DebugCamera> debugCamera_;
	// フォローカメラ
	std::unique_ptr<FollowCamera> followCamera_;
	// ベースカメラ
	std::unique_ptr<BaseCamera> baseCamera_;

	// 仮のオブジェクト
	std::unique_ptr<Model> model_;
	std::unique_ptr<Model> skydomeModel_;
	std::unique_ptr<Model> goalModel_;
	// パーツモデル
	std::unique_ptr<Model> modelBody_;
	std::unique_ptr<Model> modelR_arm_;
	std::unique_ptr<Model> modelL_arm_;
	// オブジェクト
	std::unique_ptr<SkyDome> skydome_;
	std::unique_ptr<Player> player_;
	std::unique_ptr<Enemy> enemy_;
	std::unique_ptr<Goal> goal_;
	std::unique_ptr<GroundManager> groundManager_;

	bool isDebug_ = false;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};


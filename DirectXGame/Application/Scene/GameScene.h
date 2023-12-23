#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

#include "CollisionManager.h"
#include "Missile/MissileManager.h"
#include "FollowCamera.h"
#include "IScene.h"
#include "BaseCamera.h"

#include "Player.h"
#include "LockOn.h"
#include "Enemy/EnemyManager.h"

#include "SkyDome.h"

#include <memory>

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene : public IScene {

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
	std::unique_ptr<Player> player_;
	std::unique_ptr<MissileManager> missileManager_;
	std::unique_ptr<EnemyManager> enemyManager_;

private:	// メンバポインタ
	// カメラ
	// フォローカメラ
	std::unique_ptr<FollowCamera> followCamera_;
	// ベースカメラ
	std::unique_ptr<BaseCamera> baseCamera_;

	// 仮のオブジェクト
	std::unique_ptr<Model> model_;

	bool isDebug_ = false;

	float frame_ = 0;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};


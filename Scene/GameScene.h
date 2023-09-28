#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "FollowCamera.h"
#include <memory>
#include "BaseScene.h"
#include "BaseCamera.h"

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

	void CameraUpdate();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	ViewProjection viewProjection_;

private:	// メンバポインタ
	// カメラ
	//std::unique_ptr<DebugCamera> debugCamera_;
	// フォローカメラ
	std::unique_ptr<FollowCamera> followCamera_;
	// ベースカメラ
	std::unique_ptr<BaseCamera> baseCamera_;

	// 仮のオブジェクト
	std::unique_ptr<Model> model_;

	WorldTransform baseWorld_;

	uint32_t soundFan_ = 0u;

	uint32_t soundT_ = 0u;

	uint32_t tex_ = 0u;

	bool isDebug_ = false;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};


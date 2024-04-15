#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "3D/Drawer/Instancing.h"

#include "CollisionManager.h"
#include "IScene.h"
#include "BaseCamera.h"

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
private:
	std::unique_ptr<CollisionManager> colliderManager_;
private:	// メンバポインタ

	std::unique_ptr<Instancing> particles_;

	// ベースカメラ
	std::unique_ptr<BaseCamera> baseCamera_;

	bool isDebug_ = false;

	float frame_ = 0;

	int DefeatCount_ = 0;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};


#pragma once
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Player.h"

class FollowCamera {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 追従ターゲット設定
	/// </summary>
	/// <param name="target"></param>
	void SetTarget(const WorldTransform* target);

	void SetPlayer(Player* player) { targetObject_ = player; }

	ViewProjection GetView() { return viewProjection_; }

	ViewProjection* GetViewPlayer() { return &viewProjection_; }

	void SetRotate(const Vector3& rotate) { viewProjection_.rotation_ = rotate; }

	void Reset();

	void ApplyGlobalVariables();

public:
	enum class CameraView {
		kNormal,
		kShoot,
	};

	CameraView cameraState_ = CameraView::kNormal;

private:
	// 追従対象
	const WorldTransform* target_ = nullptr;

	Player* targetObject_ = nullptr;

	// ビュープロジェクション
	ViewProjection viewProjection_;

	// デフォルトのオフセット
	Vector3 defaultOffset = {};

	// 追従対象の残像座標
	Vector3 interTarget_ = {};

	// 目標地点の角度
	float destinationAngleY_ = 0.0f;

	Vector3 defaultTranslate_;

	float delayRate_ = 0.1f;

private:
	Vector3 SetOffset() const;

};
#pragma once
#include "ViewProjection.h"
#include "WorldTransform.h"

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
	void SetTarget(const WorldTransform* target) { target_ = target; }

	ViewProjection GetView() { return viewProjection_; }

	ViewProjection* GetViewPlayer() { return &viewProjection_; }

	void SetRotate(const Vector3& rotate) { viewProjection_.rotation_ = rotate; }

	void Reset();

public:
	enum class CameraView {
		kNormal,
		kShoot,
	};

	CameraView cameraState_ = CameraView::kNormal;

private:
	// 追従対象
	const WorldTransform* target_ = nullptr;

	// ビュープロジェクション
	ViewProjection viewProjection_;

	// デフォルトのオフセット
	Vector3 defaultOffset = { 0.0f, 2.0f, -20.0f };

	// カメラまでのオフセット
	Vector3 offset;

	Vector3 interTarget_ = {};

	float destinationAngleY_ = 0.0f;

	Vector3 defaultTranslate_;

private:
	Vector3 SetOffset() const;

	bool isReset_ = false;

	float reset_t_ = 0;

};
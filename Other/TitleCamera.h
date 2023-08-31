#pragma once
#include "ViewProjection.h"
#include "WorldTransform.h"

class TitleCamera {
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

private:
	// 追従対象
	const WorldTransform* target_ = nullptr;

	// ビュープロジェクション
	ViewProjection viewProjection_;

	// カメラまでのオフセット
	Vector3 offset;

	WorldTransform worldtransform_;

};
#pragma once
#include "ViewProjection.h"
#include "WorldTransform.h"

class BaseCamera
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

public: // 設定・取得

	ViewProjection GetView() { return viewProjection_; }

	ViewProjection* GetViewPlayer() { return &viewProjection_; }

	void SetPosition(Vector3& pos) { viewProjection_.translate_ = pos; }

	void SetRotation(Vector3& rot) { viewProjection_.rotation_ = rot; }

	void SetScale(Vector3& scale) { viewProjection_.scale_ = scale; }
private:

	ViewProjection viewProjection_;

};


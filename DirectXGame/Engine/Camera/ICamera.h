#pragma once
#include "ViewProjection.h"

class ICamera
{
public:
	virtual void Initialize();

	virtual void Update();

	virtual void ImGuiWidget() = 0;

public:
#pragma region セッターとゲッター
	/// <summary>
	/// ビューの取得
	/// </summary>
	/// <returns></returns>
	ViewProjection GetView() { return viewProjection_; }

	/// <summary>
	/// アドレス用
	/// </summary>
	/// <returns></returns>
	ViewProjection* GetViewPlayer() { return &viewProjection_; }

	/// <summary>
	/// 座標設定
	/// </summary>
	/// <param name="pos"></param>
	void SetPosition(const Vector3& pos) { viewProjection_.translate_ = pos; }

	/// <summary>
	/// 角度設定
	/// </summary>
	/// <param name="rot"></param>
	void SetRotation(const Vector3& rot) { viewProjection_.rotation_ = rot; }

	/// <summary>
	/// スケール設定
	/// </summary>
	/// <param name="scale"></param>
	void SetScale(const Vector3& scale) { viewProjection_.scale_ = scale; }

	/// <summary>
	/// 視野角設定
	/// </summary>
	/// <param name="range"></param>
	void SetFov(float range) { fov_ = range; }
#pragma endregion

protected:
	ViewProjection viewProjection_;

	float fov_ = 45.0f;

protected:


};


#pragma once
#include "IScene.h"
#include "MathCalc.h"


class TestScene : public IScene
{
public:
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

	void ImGuiMatrixPrintf(const Matrix4x4& matrix, const char* tag);


};


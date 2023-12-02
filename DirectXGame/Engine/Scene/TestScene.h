#pragma once
#include "IScene.h"
#include "MathCalc.h"
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "AudioManager.h"
#include <memory>


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

private:
	/// <summary>
	/// 4x4行列表示
	/// </summary>
	/// <param name="matrix"></param>
	/// <param name="tag"></param>
	void ImGuiMatrixPrintf(const Matrix4x4& matrix, const char* tag);

	/// <summary>
	/// Vector3表示
	/// </summary>
	/// <param name="vector"></param>
	/// <param name="tag"></param>
	void ImGuiVector3Printf(const Vector3& vector, const char* tag);

	/// <summary>
	/// Quaternion表示
	/// </summary>
	/// <param name="quat"></param>
	/// <param name="tag"></param>
	void ImGuiQuaternionPrintf(const Quaternion& quat, const char* tag);

private:
	AudioManager* audio_;

	WorldTransform testTransform_;
	ViewProjection view_;

	std::unique_ptr<Model> testModel_;

};


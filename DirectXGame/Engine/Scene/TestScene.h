#pragma once
#include "IScene.h"
#include "MathCalc.h"
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "AudioManager.h"
#include "Sprite.h"
#include <memory>
#include "Editor/Editor.h"
#include "Input.h"


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

	void GetMousePosition();

	void PositionSave(Editor::HierarchicalName& names, std::string Key);

	void EditTest();

private:
	AudioManager* audio_;
	Input* input_ = nullptr;

	WorldTransform testTransform_;
	WorldTransform objTransform_;
	ViewProjection view_;

	std::unique_ptr<Model> testModel_;
	std::unique_ptr<Model> obj2Model_;
	std::unique_ptr<Model> planeModel_;
	std::unique_ptr<Sprite> testSprite_;

	uint32_t texture_ = 0u;

	float alphaValue_;

	int32_t testValue_;
	float editTest = 3.0f;

	int32_t TexValue;

	EnemyInfo enemyInfo1_;

	Vector3 savePoint_ = {};

	Vector3 nowPoint_ = {};

	void ApplyGlobalVariables();
};


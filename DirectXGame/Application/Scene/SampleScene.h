#pragma once
#include "IScene.h"
#include "Drawer/Instancing.h"
#include "Player.h"
#include "Sample/SamplePlayer.h"
#include "Sample/SampleMissile.h"
#include "Drawer/IEmitter.h"

#include <list>

class SampleScene : public IScene
{
public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	SampleScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~SampleScene();

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

	void ImGuiUpdate();

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
	Instancing* inst_;

	WorldTransform plWTF_;

	std::unique_ptr<IEmitter> emitter_;

	std::unique_ptr<Model> testModel_;

	std::unique_ptr<SamplePlayer> target_;

	std::unique_ptr<Model> assimpTest_;

	WorldTransform assimpTrf;

	std::list<SamplePlayer*> targetObjs_;

	void RegisterList(const Vector3& position);

	Vector3 newPoint = {};

	uint32_t texture_[2];

};


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
	Instancing* inst_;

	WorldTransform plWTF_;
	WorldTransform secondWTF_;

	std::unique_ptr<IEmitter> emitter_;
	std::unique_ptr<IEmitter> emitter2_;

	std::unique_ptr<Model> testModel_;
	std::unique_ptr<Model> secondModel_;

	std::unique_ptr<SamplePlayer> target_;

	std::list<SamplePlayer*> targetObjs_;

	void RegisterList(const Vector3& position);

	Vector3 newPoint = {};

	uint32_t texture_[2];

	uint32_t selectNumber_ = 0;

};


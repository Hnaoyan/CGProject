#pragma once
#include "WindowAPI.h"
#include "DirectXCommon.h"
#include "Audio.h"
#include "Model.h"
#include "Particle.h"
#include "Sprite.h"
#include "Graphics/Shader.h"
#include "AbstractSceneFactory.h"
#include "ManagerList.h"
#include <memory>

class Framework
{
public:
	/// <summary>
	/// 全体の処理をまとめる関数
	/// </summary>
	void Execute();

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize();

	/// <summary>
	/// 終了処理
	/// </summary>
	virtual void Finalize();

	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update();
	
	/// <summary>
	/// 描画処理
	/// </summary>
	virtual void Draw() = 0;

	/// <summary>
	/// 終了チェック
	/// </summary>
	/// <returns></returns>
	virtual bool IsEndRequest() { return endRequest_; }

public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~Framework() = default;

protected:

	bool endRequest_;

protected:
	/// <summary>
	/// ポインタ系
	/// </summary>
	WindowAPI* winApp;
	DirectXCommon* dxCommon;
	Audio* audio;
	Input* input;

	DescriptorManager* descriptorManager_ = nullptr;
	SceneManager* sceneManager_ = nullptr;
	ImGuiManager* imguiManager_ = nullptr;
	TransitionManager* transitionManager_ = nullptr;

	AbstractSceneFactory* sceneFactory_ = nullptr;

	std::unique_ptr<AudioManager> audioManager_;
};


template<class T> inline void SafeDelete(T*& p) {
	delete p;
	p = nullptr;
}

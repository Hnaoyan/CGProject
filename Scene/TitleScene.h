#pragma once
#include "BaseScene.h"
#include "Audio.h"
#include "Input.h"
#include "DirectXCommon.h"
#include "ViewProjection.h"

#include "Sprite.h"
#include "Model.h"
#include "Ground.h"
#include "FollowCamera.h"
#include "TitleCamera.h"

#include <memory>

class TitleScene : public BaseScene
{
public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	TitleScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~TitleScene();

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

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	ViewProjection viewProjection_;

	Sprite* title_;

	Sprite* tiles_[28];

	Sprite* titleFont_;

private:
	std::unique_ptr<Ground> ground_;
	std::unique_ptr<Model> groundModel_;

	std::unique_ptr<FollowCamera> followCamera_;

	std::unique_ptr<TitleCamera> titleCamera_;

};


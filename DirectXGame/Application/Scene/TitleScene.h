#pragma once
#include "IScene.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Sprite.h"
#include <memory>

class TitleScene : public IScene
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

	std::unique_ptr<Sprite> buttonSprite_;
	SpriteInfo buttonInfo_;

	std::unique_ptr<Sprite> back_;
	std::unique_ptr<Sprite> textSprite_;
	SpriteInfo textInfo_;

	Vector2 waveVelocity_ = {};
	float waveAnimation_t_ = 0;

};


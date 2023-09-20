#pragma once
#include "StructManager.h"
#include "Model.h"
#include <memory>

class Particle
{
public:

	void Initialize();

	void Update();

	void Draw();

public: // 設定・取得
	/// <summary>
	/// フラグの取得
	/// </summary>
	bool IsDead() { return isDead_; }
	/// <summary>
	/// パーティクルの消える時間を設定
	/// </summary>
	/// <param name="count"></param>
	void SetFadeTimer(int count) { fadeTime_ = count; }

private:
	std::unique_ptr<Model> model_;
	uint32_t texture_ = 0u;

	Vector2 position_{};
	Vector2 velocity_{};

	bool isDead_ = false;

	float radius_;

	int fadeTime_ = 0;
	int timeElapsed_ = 0;
};


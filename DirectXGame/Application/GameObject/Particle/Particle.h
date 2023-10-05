#pragma once
#include "StructManager.h"
#include "Model.h"
#include <memory>

class Particle
{
public:

	void Initialize(Model* model,uint32_t texture);

	void Update();

	void Draw(ViewProjection& viewProjection);

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

	void SetPosition(Vector3& pos) { worldTransform_.translation_ = pos; }

private:
	Model* model_;
	WorldTransform worldTransform_;

	uint32_t texture_ = 0u;

	bool isDead_ = false;

	float radius_;

	int fadeTime_ = 0;
	int timeElapsed_ = 0;
};


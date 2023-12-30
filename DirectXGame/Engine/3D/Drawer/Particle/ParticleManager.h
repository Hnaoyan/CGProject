#pragma once
#include "StructManager.h"
#include "Particle.h"
#include <list>

class ParticleManager
{
public:
	/// <summary>
	/// シングルトンインスタンス
	/// </summary>
	/// <returns></returns>
	static ParticleManager* GetInstance();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// パーティクルの更新処理
	/// </summary>
	void ParticleUpdate();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw(ViewProjection& viewProjection);

private:


	uint32_t texture_ = 0u;

	std::unique_ptr<Model> model_;


public:

	enum Type {

	};

};


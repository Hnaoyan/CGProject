#pragma once
#include "StructManager.h"
#include "ViewProjection.h"
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
	void Initialize(ViewProjection* view);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw(ViewProjection& viewProjection);

	/// <summary>
	/// ランダムエフェクト
	/// </summary>
	/// <param name="point"></param>
	void RandomRespown(const Vector3& point);

private:
	/// <summary>
	/// パーティクルの更新処理
	/// </summary>
	void ParticleUpdate();

	/// <summary>
	/// パーティクルの追加（板ポリ
	/// </summary>
	/// <param name="position"></param>
	/// <param name="velocity"></param>
	void AddParticle2D(Vector3& position, Vector3& velocity);
	/// <summary>
	/// パーティクルの追加（ブロック
	/// </summary>
	/// <param name="position"></param>
	/// <param name="velocity"></param>
	void AddParticle3D(Vector3& position,Vector3& velocity);

private:

	std::list<Particle*> particles_;

	uint32_t texture_ = 0u;

	std::unique_ptr<Model> model_;

	ViewProjection* view_;

public:

	enum Type {

	};

};


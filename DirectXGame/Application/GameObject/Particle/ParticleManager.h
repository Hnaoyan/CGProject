#pragma once
#include "StructManager.h"
#include "IParticle.h"
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
	/// 描画処理
	/// </summary>
	void Draw(ViewProjection& viewProjection);

private:
	/// <summary>
	/// パーティクルの更新処理
	/// </summary>
	void ParticleUpdate();

	std::list<IParticle*> particles_;

	uint32_t texture_ = 0u;

	std::unique_ptr<Model> cubeModel_;
	std::unique_ptr<Model> planeModel_;


public: // 外部呼び出し用関数
	// 追加
	void AddParitcle(const Vector3& position, const Vector3& velocity, const ViewProjection* viewProjection);
	void AddParitcle(const Vector3& position, uint32_t fadeTimer, const ViewProjection* viewProjection);
	// リストクリア
	void ClearList();

	void DamageEffect(const Vector3& position, const ViewProjection* viewProjection);

};


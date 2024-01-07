#pragma once
#include "StructManager.h"
#include "ViewProjection.h"
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

	std::list<IParticle*> particles_;

	ViewProjection* viewPtr_ = nullptr;

public:

	void AddParticle(const Vector3& position, uint32_t fadeTimer);

	void SetView(ViewProjection* viewPtr) {
		viewPtr_ = nullptr;
		viewPtr_ = viewPtr;
	}

	void ClearList() { particles_.clear(); }

};


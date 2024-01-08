#pragma once
#include "IMissile.h"
#include "Particle/ParticleManager.h"
#include <list>
#include <memory>

class IMissile;

/// <summary>
/// ミサイル軌道の種類
/// </summary>
enum MissileType {
	kSlerp,	// スラープ
	kV1,	// 遠心力を使って処理
	kProt4,	// シンプルなやつ
	kProt6,	// 距離に合わせるやつ
	kProt7, // オフセット値で角度調整できるやつ
	kNone,
};
/// <summary>
/// ミサイル管理クラス
/// </summary>
class MissileManager
{
public: // サブクラス
	struct MissileConfig {
		Vector3 position;
		Vector3 direct;
		Enemy* ptr;
	};
	struct ControlParam {
		float lerpRad;	// 最大遠心力
		float damping;	// 推進力
	};

public:
	/// <summary>
	/// シングルトン
	/// </summary>
	static MissileManager* GetInstance() {
		static MissileManager instance;
		return &instance;
	}

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection"></param>
	void Draw(ViewProjection& viewProjection);
public:
	void AddMissile(const MissileConfig info);

	void AddMissileNoneType(const MissileConfig info, int type);

	std::list<IMissile*> GetList() { return missiles_; }

	void SetParticleManager(ParticleManager* manager) { particleManager_ = manager; }

	// 一斉射撃Ver.1
	void BurstTheGravity(const MissileConfig info);

private:
	std::list<IMissile*> missiles_;

	std::unique_ptr<Model> model_;

	ParticleManager* particleManager_ = nullptr;

	float bulletSpeed_ = 15.0f;

	int missileType_ = 0;

	ControlParam param_ = {};
};


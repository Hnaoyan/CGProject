#pragma once
#include "IMissile.h"
#include "Particle/ParticleManager.h"
#include "Drawer/Instancing.h"
#include <list>
#include <memory>

class IMissile;
class EnemyManager;

/// <summary>
/// ミサイル軌道の種類
/// </summary>
enum MissileType {
	kSlerp,	// スラープ
	kV1,	// 遠心力を使って処理
	kProt4,	// シンプルなやつ
	kProt6,	// 距離に合わせるやつ
	kProt7, // オフセット値で角度調整できるやつ
	kV2,	// V1を途中で終了するやつ
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

	void ImGuiUpdate();


	void ListUpdate();

	void SetEnemyPtr(EnemyManager* manager) {
		enemyManager_ = manager;
	}

public:
	void AddMissile(const MissileConfig info);

	void AddMissileNoneType(const MissileConfig info, int type);

	std::list<IMissile*> GetList() { return missiles_; }

	void SetParticleManager(ParticleManager* manager) { particleManager_ = manager; }

	// 一斉射撃Ver.1
	void BurstTheGravity(const MissileConfig info);

	void Ashen(const MissileConfig info);

	void SetPosition(const Vector3& position) { playerPosition_ = position; }

	void SetInstancing(Instancing* instancing) {
		particlePtr_ = instancing;
	}

private:
	// 分離後のミサイル出現
	void SilhouetteDance();

private:
	std::list<IMissile*> missiles_;

	std::unique_ptr<Model> model_;

	ParticleManager* particleManager_ = nullptr;

	float bulletSpeed_ = 15.0f;

	int missileType_ = 0;

	ControlParam param_ = {};

	Vector3 playerPosition_ = {};

	EnemyManager* enemyManager_ = nullptr;

	Enemy* targetPtr_ = nullptr;

	bool isSeparate_ = false;

	int separateTime_ = 0;

	Instancing* particlePtr_ = nullptr;

	bool isFrameStop_ = false;
};


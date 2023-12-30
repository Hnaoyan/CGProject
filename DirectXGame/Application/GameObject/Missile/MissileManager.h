#pragma once
#include "IMissile.h"
#include <list>
#include <memory>

/// <summary>
/// ミサイル軌道の種類
/// </summary>
enum MissileType {
	eSlerp,
	eItano,
	eV1,
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
		float kSpeed;
		Enemy* ptr;
		int type;
	};

	struct ControlParam {

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

private:
	std::list<IMissile*> missiles_;

	std::unique_ptr<Model> model_;

	float bulletSpeed_ = 15.0f;
};


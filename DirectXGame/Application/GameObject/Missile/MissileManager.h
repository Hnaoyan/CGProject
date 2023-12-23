#pragma once
#include "IMissile.h"
#include <list>
#include <memory>

class MissileManager
{
public: // サブクラス
	struct AddInfo {
		Vector3 position;
		Vector3 direct;
		float kSpeed;
		Enemy* ptr;
	};
public:
	static MissileManager* GetInstance() {
		static MissileManager instance;
		return &instance;
	}

public:
	void Initialize();
	void Update();
	void Draw(ViewProjection& viewProjection);

public:
	void AddMissile(const AddInfo info);

private:
	std::list<IMissile*> missiles_;

	std::unique_ptr<Model> model_;

	float bulletSpeed_ = 15.0f;
};


#pragma once
#include "Enemy.h"
#include <list>
#include <memory>

class EnemyManager
{
public: // サブクラス
	struct AddInfo {
		Vector3 position;
		Vector3 direct;
		float kSpeed;
	};

public:
	void Initialize();
	void Update();
	void Draw(ViewProjection& viewProjection);

public:
	void AddEnemy(const AddInfo info);
	Enemy* GetTestPtr() { return testEnemy_.get(); }
	Vector3 GetTestWorld() { return testEnemy_->GetWorldPosition(); }

	std::list<Enemy*> GetList() { return enemys_; }

	int GetHP() { return hitPoint_; }

private:
	std::list<Enemy*> enemys_;
	std::unique_ptr<Enemy> testEnemy_;

	std::unique_ptr<Model> model_;
	std::unique_ptr<Model> propModel_;

	int hitPoint_ = 0;

};


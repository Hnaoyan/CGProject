#pragma once
#include "Ground.h"
#include "ViewProjection.h"
#include "CollisionManager.h"

#include <list>
#include <memory>

enum GroundType {
	kNone,
	kMove,
};

class GroundManager
{
private:
	std::list<Ground*> grounds_;

	std::unique_ptr<Model> groundModel_;

	CollisionManager* manager_;

public:
	static GroundManager* GetInstance();

	void Initialize();

	void Update();

	void Draw(const ViewProjection& view);

	void AddGround(const Vector3& pos, const Vector3& rad, const Vector3& scale);

	void AddMoveGround(const Vector3& pos, const Vector3& rad, const Vector3& scale);
	
	void SetManager(CollisionManager* manager) { manager_ = manager; }

	/// <summary>
	/// リストの取得
	/// </summary>
	/// <returns></returns>
	std::list<Ground*> GetList() { return grounds_; }
};


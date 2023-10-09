#pragma once
#include "Ground.h"
#include "ViewProjection.h"

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

public:
	static GroundManager* GetInstance();

	void Initialize();

	void Update();

	void Draw(const ViewProjection& view);

	void AddList();

	void AddGround(const Vector3& pos, const int type);
	
	/// <summary>
	/// リストの取得
	/// </summary>
	/// <returns></returns>
	std::list<Ground*> GetList() { return grounds_; }
};


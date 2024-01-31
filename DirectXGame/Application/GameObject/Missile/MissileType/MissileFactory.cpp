#include "MissileFactory.h"
#include "MissileList.h"

IMissile* MissileFactory::CreateMissile(const std::string& name, const Vector3& position)
{
	// 作成するミサイル
	IMissile* newMissile = nullptr;

	if (name == "Slerp") {
		newMissile = new SlerpMissile();
	}

	position;
	//newMissile->Initialize()
	//newMissile->SetPosition(position);

	return newMissile;
}

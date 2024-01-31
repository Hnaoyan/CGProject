#pragma once
#include "IMissile.h"
#include <string>

class MissileFactory
{
public:
	IMissile* CreateMissile(const std::string& name, const Vector3& position);


};
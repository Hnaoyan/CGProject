#pragma once
#include "Missile/IMissile.h"

class IGuidanceState
{
public:

	virtual void Update() = 0;

protected:
	IMissile* missile_;

};


#pragma once
#include "StructManager.h"

class CircleShadow
{
public:

	struct ConstBufferData
	{
		Vector4 dir;
		Vector3 casterPos;
		float distanceCasterLight;
		Vector3 atten;
		float pad3;
		Vector2 factorAngleCos;
		unsigned int active;
		float pad4;
	};

};


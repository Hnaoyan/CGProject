#pragma once
#include "StructManager.h"
#include "ViewProjection.h"

struct ParticleData
{
	Vector3 translation_ = { 0,0,0 };
	Vector3 rotation_ = { 0,0,0 };
	Vector3 scale_ = { 1,1,1 };
	Vector3 velocity_{};
	Matrix4x4 matWorld_ = {};

	ViewProjection* viewProjection_ = nullptr;

	float lifeTime_ = 0;
	bool isAlive_ = false;
	bool isBillBoard_ = false;

	void UpdateMatrix();
	void SettingBillBoard(ViewProjection* viewProjection) {
		viewProjection_ = viewProjection;
		isBillBoard_ = true;
	}

};


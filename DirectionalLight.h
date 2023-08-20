#pragma once
#include "StructManager.h"

class DirectionalLight
{

public:
	struct ConstBufferData 
	{
		Vector4 lightVector;
		Vector3 lightColor;
		unsigned int active;
	};

public:
	inline void SetLightDir(const Vector4& lightDir) { this->lightDir_ = lightDir; }

	inline const Vector4& GetLightDir() { return lightDir_; }

	inline void SetLightColor(const Vector3& lightColor) { this->lightColor_ = lightColor; }

	inline const Vector3& GetLightColor() { return lightColor_; }

	inline void SetActive(bool active) { this->active_ = active; }

	inline bool IsActive() { return active_; }

private:
	// ライトの方向
	Vector4 lightDir_ = { 1,0,0,0 };
	// ライトの色
	Vector3 lightColor_ = { 1,1,1 };
	// ライトのフラグ
	bool active_ = false;

};


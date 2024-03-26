#pragma once
#include "StructManager.h"

class IEmitter
{
public:

	IEmitter() {};
	~IEmitter() = default;

	struct TransformInstance {
		Vector3 scale;
		Vector3 rotate;
		Vector3 translate;
	};


	TransformInstance transform;	// 座標
	float frequency_;	 // 頻度
	float frequencyTime_; // 頻度用時刻

	float frequencyFrame_;
	uint32_t count_;		// 発生数

private:
	float lifeTime_;	// 時間
	float limitTime_;	// 制限時間


public:

	void Initialize(uint32_t count,float frequency);
	void Update(const Vector3& position);
	
	void Reset();

};


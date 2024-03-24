#include "IEmitter.h"

void IEmitter::Initialize(uint32_t count, float frequency)
{
	// 数
	count_ = count;
	// 頻度
	frequency_ = frequency;
	// 頻度カウントリセット
	frequencyTime_ = 0;

	//frequencyFrame_ = 1.0f;

	// トランスフォームの初期化
	transform.scale = { 1,1,1 };
	transform.rotate = {};
	transform.translate = {};

}

void IEmitter::Update(const Vector3& position)
{
	// 座標更新
	transform.translate = position;

	frequencyTime_ += (1.0f / frequency_);

}

void IEmitter::Reset()
{

	frequencyTime_ = 0;

}

#include "IParticle.h"

void IParticle::Initialize(Model* model, uint32_t texture) 
{
	// モデル設定
	model_ = model;
	// テクスチャ設定
	texture_ = texture;
	// 座標系設定
	worldTransform_.Initialize();
	worldTransform_.scale_ = { 0.25f,0.25f,0.25f };
	model_->SetAlphaValue(0.3f);
	this->timeElapsed_ = 0;
}

void IParticle::Update()
{

	if (++timeElapsed_ > fadeTime_) {
		isDead_ = true;
	}

	worldTransform_.UpdateMatrix();

}


void IParticle::Draw(ViewProjection& viewProjection)
{
	model_->SetBlendMode(Model::BlendMode::kAdd);
	model_->Draw(worldTransform_, viewProjection, texture_);
}
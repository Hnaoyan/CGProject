#include "IParticle.h"
#include "MathCalc.h"

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
	ease_t_ = 0;
}

void IParticle::Update()
{
	timeElapsed_++;

	if (timeElapsed_ > fadeTime_) {
		isDead_ = true;
	}

	ease_t_ += 1.0f / (float)fadeTime_;
	if (ease_t_ >= 1.0f) {
		ease_t_ = 1.0f;
	}

	worldTransform_.scale_ = MathCalc::EaseInCubicF(ease_t_, Vector3(0.25f, 0.25f, 0.25f), Vector3(0, 0, 0));
	worldTransform_.translation_ += velocity_;
	worldTransform_.UpdateMatrix();

}


void IParticle::Draw(ViewProjection& viewProjection)
{
	model_->SetBlendMode(Model::BlendMode::kAdd);
	model_->Draw(worldTransform_, viewProjection, texture_);
}
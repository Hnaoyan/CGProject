#include "Particle.h"

void Particle::Initialize(Model* model, uint32_t texture) 
{
	// モデル設定
	model_ = model;
	// テクスチャ設定
	texture_ = texture;
	// 座標系設定
	worldTransform_.Initialize();
}

void Particle::Update() 
{

	worldTransform_.UpdateMatrix();

}


void Particle::Draw(ViewProjection& viewProjection) 
{
	model_->Draw(worldTransform_, viewProjection, texture_);
}
#include "UIBase.h"

void UIBase::Initialize(int texHandle)
{
	textureHandle_ = texHandle;
	sprite_.reset(Sprite::Create(textureHandle_, position_, color_, anchorPoint_, false, false));
	size_ = sprite_->GetSize();
}

void UIBase::Update()
{
	//sprite_->
}

void UIBase::Draw()
{
	// 描画
	sprite_->Draw();
}

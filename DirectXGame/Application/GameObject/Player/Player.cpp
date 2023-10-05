#include "Player.h"

void Player::Initialize(Model* model)
{
	// モデル読み込み
	radius_ = 10.0f;
	BaseCharacter::Initialize(model);
	
}

void Player::Update()
{
}

void Player::Draw(const ViewProjection& viewProjection)
{
	// ベースの描画
	BaseCharacter::Draw(viewProjection);

}

#include "RectangleCollider.h"

void RectangleCollider::Initialize(WorldTransform* worldTransfrom, Vector2 size)
{

	// ワールドトランスフォーム
	worldTransform_ = worldTransfrom;

	// 位置
	position_ = Vector2{ worldTransfrom->matWorld_.m[3][0], worldTransfrom->matWorld_.m[3][1]};

	// サイズ
	size_ = size;

}

void RectangleCollider::Update(Vector2 position)
{

	// 位置
	position_ = position;

}

void RectangleCollider::OnCollision(uint32_t collisonObj, WorldTransform* worldTransfrom)
{

	function_(collisonObj, worldTransfrom);

}

#include "LockOn.h"
#include "TextureManager.h"
#include "StructManager.h"
#include "MathCalc.h"

void LockOn::Initialize()
{
}

void LockOn::Update(const std::list<std::unique_ptr<Enemy>>& enemies, const ViewProjection& viewProjection)
{
	//if (target_) {

	//}

	//// 目標
	//std::list<std::pair<float, const Enemy*>> targets;

	//// 全ての敵に対して順にロックオン判定
	//for (const std::unique_ptr<Enemy>& enemy : enemies) {
	//	// 敵のロックオン座標取得
	//	Vector3 positionWorld = enemy->GetWorldPosition();

	//	// ワールド→ビュー座標変換
	//	Vector3 positionView = MathCalc::TransformNormal(positionWorld, viewProjection.matView);

	//	// 距離条件チェック
	//	if (minDistance_ <= positionView.z && positionView.z <= maxDistance_) {

	//		// カメラ前方との角度を計算
	//		float arcTangent;
	//	}

	//}
	enemies;
	viewProjection;
}

void LockOn::Draw()
{
	if (target_) {
		lockOnMark_->Draw();
	}
}

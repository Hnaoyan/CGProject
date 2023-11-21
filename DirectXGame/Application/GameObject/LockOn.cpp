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
	enemies;
	viewProjection;
	SearchEnemy(enemies, viewProjection);
}

void LockOn::SearchEnemy(const std::list<std::unique_ptr<Enemy>>& enemies, const ViewProjection& viewProjection)
{

	// 目標
	std::list<std::pair<float, const Enemy*>> targets;

	// 全ての敵に対して順にロックオン判定
	for (const std::unique_ptr<Enemy>& enemy : enemies) {
		// 敵のロックオン座標取得
		Vector3 positionWorld = enemy->GetWorldPosition();

		// ワールド→ビュー座標変換
		Vector3 positionView = MathCalc::TransformNormal(positionWorld, viewProjection.matView);

		// 距離条件チェック
		if (minDistance_ <= positionView.z && positionView.z <= maxDistance_) {

			// カメラ前方との角度を計算
			float arcTangent = std::atan2f(std::sqrtf(positionView.x * positionView.x + positionView.y * positionView.y), positionView.z);

			// 角度条件チェック
			if (std::fabsf(arcTangent) <= angleRange_) {
				targets.emplace_back(std::make_pair(positionView.z, enemy.get()));
			}

		}

	}

	// 対象をリセット
	target_ = nullptr;
	if (!targets.empty()) {
		// 昇順にソート
		targets.sort([](auto& pair1, auto& pair2) {return pair1.first < pair2.first; });
		// ソートの結果一番近い敵をロックオン対象に設定
		target_ = targets.front().second;
	}


}

void LockOn::Draw()
{
	if (target_) {
		lockOnMark_->Draw();
	}
}

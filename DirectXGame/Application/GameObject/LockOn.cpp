#include "LockOn.h"
#include "TextureManager.h"
#include "StructManager.h"
#include "TextureManager.h"
#include "MathCalc.h"
#include "WindowAPI.h"
#include "Input.h"

void LockOn::Initialize()
{
	uint32_t texture = TextureManager::Load("lockOn.png");
	position_ = { (float)WindowAPI::kClientWidth / 2 , (float)WindowAPI::kClientHeight / 2 };
	this->lockOnMark_.reset(Sprite::Create(texture, position_, { 1,1,1,1 }, { 0.5f,0.5f }, false, false));

}

void LockOn::Update(const std::list<std::unique_ptr<Enemy>>& enemies, const ViewProjection& viewProjection)
{
	XINPUT_STATE joyState;
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_X) {
			SearchEnemy(enemies, viewProjection);
		}

		//// ロックオン解除
		//if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_Y) {
		//	target_ = nullptr;
		//}
		//// 範囲外判定
		//else if (OutOfRange(viewProjection)) {
		//	target_ = nullptr;
		//}

	}

	if (target_) {
		// 敵のロックオン座標取得
		Vector3 positionWorld = target_->GetWorldPositionTarget();
		// ワールド座標から返還
		Vector3 positionScreen = this->WorldToScreen(positionWorld, viewProjection);

		//
		Vector2 positionScreenV2(positionScreen.x, positionScreen.y);

		lockOnMark_->SetPosition(positionScreenV2);

	}

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

bool LockOn::OutOfRange(const ViewProjection& viewProjection)
{
	if (target_ == nullptr) {
		return true;
	}

	// 敵のロックオン座標取得
	Vector3 positionWorld = target_->GetWorldPositionTarget();
	// ワールド座標から返還
	Vector3 positionView = MathCalc::TransformNormal(positionWorld, viewProjection.matView);


	// 距離条件チェック
	if (minDistance_ <= positionView.z && positionView.z <= maxDistance_) {

		// カメラ前方との角度を計算
		float arcTangent = std::atan2f(std::sqrtf(positionView.x * positionView.x + positionView.y * positionView.y), positionView.z);

		// 角度条件チェック
		if (std::fabsf(arcTangent) <= angleRange_) {
			// 範囲外でない
			return false;
		}

	}

	// 範囲外である
	return true;
}

void LockOn::Draw()
{
	if (target_) {
		lockOnMark_->Draw();
	}
}

Vector3 LockOn::WorldToScreen(const Vector3& position, const ViewProjection& viewProjection)
{
	// ビューポート行列
	Matrix4x4 matViewport = MatLib::MakeViewportMatrix(0, 0,
		(float)WindowAPI::kClientWidth, (float)WindowAPI::kClientHeight, 0, 1);
	// ビュープロジェクション
	Matrix4x4 matViewProjection = MatLib::Multiply(
		MatLib::Multiply(viewProjection.matView, viewProjection.matProjection), matViewport);

	return Vector3(MatLib::Transform(position,matViewProjection));
}

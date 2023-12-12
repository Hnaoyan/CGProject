#include "LockOn.h"
#include "TextureManager.h"
#include "StructManager.h"
#include "TextureManager.h"
#include "MathCalc.h"
#include "WindowAPI.h"
#include "Input.h"
#include "imgui.h"

void LockOn::Initialize()
{
	uint32_t texture = TextureManager::Load("lockOn.png");
	position_ = { (float)WindowAPI::kClientWidth / 2 , (float)WindowAPI::kClientHeight / 2 };
	this->lockOnMark_.reset(Sprite::Create(texture, position_, { 1,1,1,1 }, { 0.5f,0.5f }, false, false));

}

void LockOn::Update(const std::list<std::unique_ptr<Enemy>>& enemies, const ViewProjection& viewProjection)
{
	
	ImGui::Begin("LockOn");
	ImGui::Text("isAuto : PAD_A : %d", isAuto_);
	ImGui::Text("targetReset : PAD_Y");
	ImGui::Text("targetSearch : PAD_X");
	ImGui::Text("target : %d", (target_));
	ImGui::DragFloat("angle", &angleValue_, 0.1f, 0, 180.0f);
	ImGui::End();

	angleRange_ = angleValue_ * kDegreeToRadian;

	XINPUT_STATE joyState;
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {

		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A) {
			if (isAuto_) {
				isAuto_ = false;
			}
			else {
				isAuto_ = true;
			}
		}
		if (target_) {
			// C.
			// ロックオン解除
			if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_Y) {
				target_ = nullptr;
			}
			// 範囲外判定
			else if (OutOfRange(target_, viewProjection)) {
				target_ = nullptr;
			}

			if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB && !isDuring_) {
				if (targetsItr_ != targets_.end()) {
					target_ = targetsItr_->second;
					++targetsItr_;
				}
				else {
					targetsItr_ = targets_.begin();
					target_ = targetsItr_->second;
				}
				isDuring_ = true;
			}

		}
		else
		{
			if (isAuto_) {
				//SearchEnemy(enemies, viewProjection);
			}
			else {
				// A.
				if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_X && !isDuring_) {
					SearchEnemy(enemies, viewProjection);
					isDuring_ = true;
				}
			}
		}
	}

	if (target_) {
		// B.
		// 敵のロックオン座標取得
		Vector3 local = { 0,1.0f,0 };
		Vector3 positionWorld = MatLib::Transform(local, target_->GetTransform().matWorld_);

		// ワールド座標から返還
		Vector3 positionScreen = this->WorldToScreen(positionWorld, viewProjection);

		//
		Vector2 positionScreenV2(positionScreen.x, positionScreen.y);

		lockOnMark_->SetPosition(positionScreenV2);

	}

	if (isDuring_) {
		coolTime_++;
		if (coolTime_ > 10) {
			coolTime_ = 0;
			isDuring_ = false;
		}
	}

}

void LockOn::SearchEnemy(const std::list<std::unique_ptr<Enemy>>& enemies, const ViewProjection& viewProjection)
{
	// ここバグ
	// 目標
	targets_.clear();

	// 全ての敵に対して順にロックオン判定
	for (const std::unique_ptr<Enemy>& enemy : enemies) {

		// 死亡の場合スキップ
		if (enemy->GetIsDead() || OutOfRange(enemy.get(), viewProjection)) {
			continue;
		}

		// 敵のロックオン座標取得
		Vector3 positionWorld = enemy->GetWorldPosition();
		//Vector3 off = { 0,1.0f,0 };
		//positionWorld = MatLib::Transform(off, enemy->GetTransform().matWorld_);

		// ワールド→ビュー座標変換
		Vector3 positionView = MatLib::Transform(positionWorld, viewProjection.matView);

		// 距離条件チェック
		if (minDistance_ <= positionView.z && positionView.z <= maxDistance_) {

			// カメラ前方との角度を計算
			//Vector3 WorldView = { viewProjection.matView.m[3][0],viewProjection.matView.m[3][1],viewProjection.matView.m[3][2] };
			//float arcTangent = MathCalc::Dot(MathCalc::Normalize(positionView), MathCalc::Normalize(WorldView));
			float arcTangent = MathCalc::CalculateYawFromVector(Vector3(std::sqrtf(positionView.x * positionView.x + positionView.y + positionView.y), 0.0f, positionView.z));
			// 角度条件チェック
			if (std::fabsf(arcTangent) <= angleRange_) {
				targets_.emplace_back(std::make_pair(positionView.z, enemy.get()));
			}
		}
	}

	// 対象をリセット
	target_ = nullptr;
	if (!targets_.empty()) {
		// 昇順にソート
		targets_.sort([](auto& pair1, auto& pair2) {return pair1.first < pair2.first; });
		// ソートの結果一番近い敵をロックオン対象に設定
		target_ = targets_.front().second;
		targetsItr_ = targets_.begin();
	}
}

bool LockOn::OutOfRange(const Enemy* target, const ViewProjection& viewProjection)
{
	// 敵のロックオン座標取得
	Vector3 positionWorld = {
		target->GetTransform().matWorld_.m[3][0],
		target->GetTransform().matWorld_.m[3][1],
		target->GetTransform().matWorld_.m[3][2] };
	// ワールド座標から返還
	Vector3 positionView = MatLib::Transform(positionWorld, viewProjection.matView);

	if (minDistance_ <= positionView.z && positionView.z <= maxDistance_) {

		// カメラ前方との角度を計算
		Vector3 WorldView = { viewProjection.matView.m[3][0],viewProjection.matView.m[3][1],viewProjection.matView.m[3][2] };
		float arcTangent = MathCalc::Dot(MathCalc::Normalize(positionView), MathCalc::Normalize(WorldView));

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

Vector3 LockOn::GetTargetPosition() const
{
	if (target_) {
		return Vector3{ target_->GetTransform().matWorld_.m[3][0],target_->GetTransform().matWorld_.m[3][1],target_->GetTransform().matWorld_.m[3][2] };
	}

	return Vector3();
}


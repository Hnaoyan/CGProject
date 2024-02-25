#pragma once
#include <memory>
#include <list>

#include "Enemy.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "Player.h"

class Player;

class LockOn
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="enemies"></param>
	/// <param name="viewProjection"></param>
	void Update(const std::list<std::unique_ptr<Enemy>>& enemies, const ViewProjection& viewProjection, Player* player);
	/// <summary>
	/// 描画
	/// </summary>
	void Draw();
	/// <summary>
	/// 検索
	/// </summary>
	/// <param name="enemies"></param>
	/// <param name="viewProjection"></param>
	void SearchEnemy(const std::list<std::unique_ptr<Enemy>>& enemies, const ViewProjection& viewProjection, Player* player);

	bool OutOfRange(const Enemy* target, const ViewProjection& viewProjection, Player* player);

	Vector3 WorldToScreen(const Vector3& position, const ViewProjection& viewProjection);

	const Enemy* GetEnemyAddress() { return target_; }

	void ResetTarget() { target_ = nullptr; }

private:
	std::unique_ptr<Sprite> lockOnMark_;

	const Enemy* target_ = nullptr;

	float kDegreeToRadian = (std::numbers::pi_v<float> / 180.0f);

	float minDistance_ = 0.0f;

	float maxDistance_ = 30.0f;

	float angleValue_ = 90.0f;

	float angleRange_ = angleValue_ * kDegreeToRadian;

	Vector2 position_ = {};

	bool isAuto_ = false;

	bool isDuring_ = false;

	bool isAutoDuring_ = false;

	uint32_t coolTime_ = 0;
	uint32_t autoCoolTime_ = 0;

	std::list<std::pair<float, const Enemy*>> targets_;
	std::list<std::pair<float, const Enemy*>>::iterator targetsItr_;

public: // アクセッサ
	Vector3 GetTargetPosition() const;
	const Enemy* GetTarget() const { return target_; }
	bool ExistTarget() const { return target_ ? true : false; }
	void SetIsAuto(bool isAuto) { isAuto_ = isAuto; }

};
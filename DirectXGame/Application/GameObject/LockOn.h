#pragma once
#include <memory>
#include <list>

#include "Enemy.h"
#include "Sprite.h"
#include "ViewProjection.h"

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
	void Update(const std::list<std::unique_ptr<Enemy>>& enemies,const ViewProjection& viewProjection);
	/// <summary>
	/// 描画
	/// </summary>
	void Draw();
	/// <summary>
	/// 検索
	/// </summary>
	/// <param name="enemies"></param>
	/// <param name="viewProjection"></param>
	void SearchEnemy(const std::list<std::unique_ptr<Enemy>>& enemies, const ViewProjection& viewProjection);
	/// <summary>
	/// 範囲外か確認
	/// </summary>
	/// <param name="viewProjection"></param>
	/// <returns></returns>
	bool OutOfRange(const ViewProjection& viewProjection);

	bool OutOfRange(Enemy* target, const ViewProjection& viewProjection);

	Vector3 WorldToScreen(const Vector3& position, const ViewProjection& viewProjection);

private:
	std::unique_ptr<Sprite> lockOnMark_;

	const Enemy* target_ = nullptr;

	float kDegreeToRadian = 90.0f * ((float)std::numbers::pi * 180.0f);

	float minDistance_ = 10.0f;

	float maxDistance_ = 50.0f;

	float angleRange_ = 20.0f * kDegreeToRadian;

	Vector2 position_ = {};

	bool isAuto_ = false;

	bool isDuring_ = false;

	uint32_t coolTime_ = 0;

public:
	Vector3 GetTargetPosition() const;
	const Enemy* GetTarget() const { return target_; }
	bool ExistTarget() const { return target_ ? true : false; }

	void SetIsAuto(bool isAuto) { isAuto_ = isAuto; }

};


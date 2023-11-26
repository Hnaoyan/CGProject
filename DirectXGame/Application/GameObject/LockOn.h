#pragma once
#include <memory>
#include <list>

#include "Enemy.h"
#include "Sprite.h"
#include "ViewProjection.h"

class LockOn
{
public:

	void Initialize();

	void Update(const std::list<std::unique_ptr<Enemy>>& enemies,const ViewProjection& viewProjection);

	void SearchEnemy(const std::list<std::unique_ptr<Enemy>>& enemies, const ViewProjection& viewProjection);

	bool OutOfRange(const ViewProjection& viewProjection);

	void Draw();

	Vector3 WorldToScreen(const Vector3& position, const ViewProjection& viewProjection);

private:
	std::unique_ptr<Sprite> lockOnMark_;

	const Enemy* target_ = nullptr;

	float kDegreeToRadian = 45.0f * ((float)std::numbers::pi * 180.0f);

	float minDistance_ = 10.0f;

	float maxDistance_ = 30.0f;

	float angleRange_ = 20.0f * kDegreeToRadian;

	Vector2 position_ = {};

public:
	Vector3 GetTargetPosition() const;
	bool ExistTarget() const { return target_ ? true : false; }
};


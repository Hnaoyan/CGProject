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

	void Draw();

private:
	std::unique_ptr<Sprite> lockOnMark_;

	const Enemy* target_ = nullptr;

	float minDistance_ = 10.0f;

	float maxDistance_ = 30.0f;

	float angleRange_ = 20.0f;
};


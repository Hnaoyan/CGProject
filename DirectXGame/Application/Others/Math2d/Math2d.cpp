#include "Math2d.h"

Vector2 Math2d::Subtruct(const Vector2& v1, const Vector2& v2)
{
	return Vector2{v2.x - v1.x, v2.y - v1.y };
}

float Math2d::Cross(const Vector2& v1, const Vector2& v2)
{
	return  v1.x * v2.y - v1.y * v2.x;
}

bool Math2d::segmentsCrossing(const Vector2& v1, const Vector2& v2, const Vector2& v3, const Vector2& v4)
{

	Vector2 v1v2 = Math2d::Subtruct(v1, v2);
	Vector2 v1v3 = Math2d::Subtruct(v1, v3);
	Vector2 v1v4 = Math2d::Subtruct(v1, v4);
	Vector2 v3v1 = Math2d::Subtruct(v3, v1);
	Vector2 v3v2 = Math2d::Subtruct(v3, v2);
	Vector2 v3v4 = Math2d::Subtruct(v3, v4);

	// 平行
	if (Math2d::Cross(v1v2, v3v4) == 0.0f) {
		return false;
	}

	bool result = (Math2d::Cross(v1v2, v1v3) * Math2d::Cross(v1v2, v1v4) < 0.0f) &&
		(Math2d::Cross(v3v4, v3v1) * Math2d::Cross(v3v4, v3v2) < 0.0f);

	return result;

}

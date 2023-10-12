#pragma once
#include <StructManager.h>

/// <summary>
///  2d数学関数(10月専用)
/// </summary>
class Math2d
{

public: //静的メンバ関数
	
	/// <summary>
	/// 減算
	/// </summary>
	/// <param name="v1"></param>
	/// <param name="v2"></param>
	/// <returns></returns>
	static Vector2 Subtruct(const Vector2& v1, const Vector2& v2);

	/// <summary>
	/// 外積
	/// </summary>
	/// <param name="v1"></param>
	/// <param name="v2"></param>
	/// <returns></returns>
	static float Cross(const Vector2& v1, const Vector2& v2);

	/// <summary>
	/// 線分がクロスしているか
	/// </summary>
	/// <param name="v1"></param>
	/// <param name="v2"></param>
	/// <param name="v3"></param>
	/// <param name="v4"></param>
	/// <returns></returns>
	static bool segmentsCrossing(const Vector2& v1, const Vector2& v2, const Vector2& v3, const Vector2& v4);
		

};


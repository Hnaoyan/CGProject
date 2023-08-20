#pragma once
#include "StructManager.h"

class Vector3Math 
{
public:
	/// <summary>
	/// 加算
	/// </summary>
	/// <param name="v1"></param>
	/// <param name="v2"></param>
	/// <returns></returns>
	static Vector3 Add(const Vector3& v1, const Vector3& v2);
	/// <summary>
	/// 減算
	/// </summary>
	/// <param name="v1"></param>
	/// <param name="v2"></param>
	/// <returns></returns>
	static Vector3 Subtract(const Vector3& v1, const Vector3& v2);

	/// <summary>
	/// スカラー倍
	/// </summary>
	/// <param name="v1"></param>
	/// <param name="scaler"></param>
	/// <returns></returns>
	static Vector3 Scaler(const Vector3& v1, const float scaler);


};

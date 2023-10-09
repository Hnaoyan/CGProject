#pragma once
#include "StructManager.h"

class VectorLib
{
public: // Vector4
	/// <summary>
	/// 加算
	/// </summary>
	/// <param name="v1"></param>
	/// <param name="v2"></param>
	/// <returns></returns>
	static Vector4 Add(const Vector4& v1, const Vector4& v2);

public: // Vector3
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
	/// 乗算
	/// </summary>
	/// <param name="scaler"></param>
	/// <param name="v"></param>
	/// <returns></returns>
	static Vector3 Multiply(float scaler, const Vector3& v);
	/// <summary>
	/// スカラー倍
	/// </summary>
	/// <param name="v1"></param>
	/// <param name="scaler"></param>
	/// <returns></returns>
	static Vector3 Scaler(const Vector3& v1, const float scaler);


};


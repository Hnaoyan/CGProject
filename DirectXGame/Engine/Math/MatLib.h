#pragma once
#include "StructManager.h"

class MatLib
{
public:
	/// <summary>
	/// ベクターと行列の計算
	/// </summary>
	/// <param name="vector"></param>
	/// <param name="matrix"></param>
	/// <returns></returns>
	static Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix);
	/// <summary>
	/// アフィン変換
	/// </summary>
	/// <param name="scale"></param>
	/// <param name="rotate"></param>
	/// <param name="translate"></param>
	/// <returns></returns>
	static Matrix4x4
		MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);
	/// <summary>
	/// X軸行列計算
	/// </summary>
	/// <param name="radian"></param>
	/// <returns></returns>
	static Matrix4x4 MakeRotateXMatrix(float radian);
	/// <summary>
	/// Y軸行列計算
	/// </summary>
	/// <param name="radian"></param>
	/// <returns></returns>
	static Matrix4x4 MakeRotateYMatrix(float radian);
	/// <summary>
	/// Z軸行列計算
	/// </summary>
	/// <param name="radian"></param>
	/// <returns></returns>
	static Matrix4x4 MakeRotateZMatrix(float radian);
	/// <summary>
	/// 行列の乗算
	/// </summary>
	/// <param name="m1"></param>
	/// <param name="m2"></param>
	/// <returns></returns>
	static Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);
	/// <summary>
	/// 逆行列
	/// </summary>
	/// <param name="m"></param>
	/// <returns></returns>
	static Matrix4x4 MakeInverse(const Matrix4x4& m);
	/// <summary>
	/// 単位行列
	/// </summary>
	/// <returns></returns>
	static Matrix4x4 MakeIdentity4x4();

	/// <summary>
	/// スケール
	/// </summary>
	/// <param name="translate"></param>
	/// <returns></returns>
	static Matrix4x4 MakeScaleMatrix(const Vector3& translate);

	/// <summary>
	/// 平行移動
	/// </summary>
	/// <param name="translate"></param>
	/// <returns></returns>
	static Matrix4x4 MakeTranslateMatrix(const Vector3& translate);

	static Matrix4x4
		MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip);

	static Matrix4x4 MakeOrthographicMatrix(
		float left, float top, float right, float bottom, float nearClip, float farClip);

	static Matrix4x4 MakeViewportMatrix(
		float left, float top, float width, float height, float minDepth, float maxDepth);

	static Vector3 GetWorldPosition(const Matrix4x4& matWorld);

	static Vector3 TransformNormal(const Vector3& v, const Matrix4x4& mat);

	/// <summary>
	/// ビルボード計算用
	/// </summary>
	/// <param name="target"></param>
	/// <param name="eye"></param>
	/// <param name="up"></param>
	/// <returns></returns>
	static Matrix4x4 MakeBillBoard(const Vector3& target, const Vector3& eye, const Vector3& up);

	/// <summary>
	/// 任意軸の回転行列を求める計算
	/// </summary>
	/// <param name="from"></param>
	/// <param name="to"></param>
	/// <returns></returns>
	static Matrix4x4 DirectToDirect(const Vector3& from, const Vector3& to);

};

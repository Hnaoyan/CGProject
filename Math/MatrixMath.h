#pragma once
#include "StructManager.h"

class MatrixMath 
{
public:
	static Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix);
	static Matrix4x4
	    MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);
	// Matrix4x4 Add(const Matrix4x4& m1, const Matrix4x4& m2);
	static Matrix4x4 Subtract(const Matrix4x4& m1, const Matrix4x4& m2);
	// Rotate
	static Matrix4x4 MakeRotateXMatrix(float radian);
	static Matrix4x4 MakeRotateYMatrix(float radian);
	static Matrix4x4 MakeRotateZMatrix(float radian);
	static Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);
	static Matrix4x4 MakeInverse(const Matrix4x4& m);
	static Matrix4x4 MakeIdentity4x4();

	// Transpose
	// Matrix4x4 Transpose(const Matrix4x4& m);
	// Scale
	static Matrix4x4 MakeScaleMatrix(const Vector3& translate);
	// Translate
	static Matrix4x4 MakeTranslateMatrix(const Vector3& translate);

	static Matrix4x4
	    MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip);

	static Matrix4x4 MakeOrthographicMatrix(
	    float left, float top, float right, float bottom, float nearClip, float farClip);

	static Matrix4x4 MakeViewportMatrix(
	    float left, float top, float width, float height, float minDepth, float maxDepth);

	static Vector3 GetWorldPosition(const Matrix4x4& matWorld);

	static Vector3 TransformNormal(const Vector3& v, const Matrix4x4& mat);

};

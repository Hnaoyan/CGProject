#pragma once
struct Matrix4x4
{
	float m[4][4];
};
struct Vector3 {
	float x, y, z;
};

struct Transform {
	Vector3 scale;
	Vector3 rotate;
	Vector3 translate;
};
struct Quaternion {
	float x, y, z, w;
};

Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);
//Matrix4x4 Add(const Matrix4x4& m1, const Matrix4x4& m2);
Matrix4x4 Subtract(const Matrix4x4& m1, const Matrix4x4& m2);
// Rotate
Matrix4x4 MakeRotateXMatrix(float radian);
Matrix4x4 MakeRotateYMatrix(float radian);
Matrix4x4 MakeRotateZMatrix(float radian);
Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);
Matrix4x4 MakeInverse(const Matrix4x4& m);
Matrix4x4 MakeIdentity4x4();

// Transpose
//Matrix4x4 Transpose(const Matrix4x4& m);
// Scale
Matrix4x4 MakeScaleMatrix(const Vector3& translate);
// Translate
Matrix4x4 MakeTranslateMatrix(const Vector3& translate);

Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip);
Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip);
Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth);

Matrix4x4 MakeRotateMatrix(const Quaternion& quaternion);
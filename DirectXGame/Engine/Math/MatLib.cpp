#include "MatLib.h"
#include "VectorLib.h"
#include "MathCalc.h"

#include <cmath>
#include <cassert>

Matrix4x4 MatLib::MakeAffineMatrix(
	const Vector3& scale, const Vector3& rotate, const Vector3& translate) {
	Matrix4x4 result = {};
	Matrix4x4 matrixX = MakeRotateXMatrix(rotate.x);
	Matrix4x4 matrixY = MakeRotateYMatrix(rotate.y);
	Matrix4x4 matrixZ = MakeRotateZMatrix(rotate.z);
	Matrix4x4 matrixRotate = Multiply(matrixX, Multiply(matrixY, matrixZ));

	result.m[0][0] = scale.x * matrixRotate.m[0][0];
	result.m[0][1] = scale.x * matrixRotate.m[0][1];
	result.m[0][2] = scale.x * matrixRotate.m[0][2];
	result.m[1][0] = scale.y * matrixRotate.m[1][0];
	result.m[1][1] = scale.y * matrixRotate.m[1][1];
	result.m[1][2] = scale.y * matrixRotate.m[1][2];
	result.m[2][0] = scale.z * matrixRotate.m[2][0];
	result.m[2][1] = scale.z * matrixRotate.m[2][1];
	result.m[2][2] = scale.z * matrixRotate.m[2][2];

	result.m[3][3] = 1.0f;
	result.m[3][0] = translate.x;
	result.m[3][1] = translate.y;
	result.m[3][2] = translate.z;

	return result;
}
Matrix4x4 MatLib::MakeRotateXMatrix(float radian) {
	Matrix4x4 result = {};
	result.m[0][0] = 1.0f;
	result.m[1][1] = std::cosf(radian);
	result.m[1][2] = std::sinf(radian);
	result.m[2][1] = std::sinf(-radian);
	result.m[2][2] = std::cosf(radian);
	result.m[3][3] = 1.0f;
	return result;
}

Matrix4x4 MatLib::MakeRotateYMatrix(float radian) {
	Matrix4x4 result = {};
	result.m[0][0] = std::cosf(radian);
	result.m[0][2] = std::sinf(-radian);
	result.m[1][1] = 1.0f;
	result.m[2][0] = std::sinf(radian);
	result.m[2][2] = std::cosf(radian);
	result.m[3][3] = 1.0f;
	return result;
}

Matrix4x4 MatLib::MakeRotateZMatrix(float radian) {
	Matrix4x4 result = {};
	result.m[0][0] = std::cosf(radian);
	result.m[0][1] = std::sinf(radian);
	result.m[1][0] = std::sinf(-radian);
	result.m[1][1] = std::cosf(radian);
	result.m[2][2] = 1.0f;
	result.m[3][3] = 1.0f;
	return result;
}

Matrix4x4 MatLib::Multiply(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 result;
	for (int y = 0; y < 4; y++) {
		for (int x = 0; x < 4; x++) {
			result.m[y][x] = 0;
			for (int k = 0; k < 4; k++) {
				result.m[y][x] += m1.m[y][k] * m2.m[k][x];
			}
		}
	}
	return result;
}

Matrix4x4 MatLib::MakeInverse(const Matrix4x4& m) {
	Matrix4x4 result = {};
	float det = 0;
	det += (m.m[0][0] * m.m[1][1] * m.m[2][2] * m.m[3][3]);
	det += (m.m[0][0] * m.m[1][2] * m.m[2][3] * m.m[3][1]);
	det += (m.m[0][0] * m.m[1][3] * m.m[2][1] * m.m[3][2]);

	det -= (m.m[0][0] * m.m[1][3] * m.m[2][2] * m.m[3][1]);
	det -= (m.m[0][0] * m.m[1][2] * m.m[2][1] * m.m[3][3]);
	det -= (m.m[0][0] * m.m[1][1] * m.m[2][3] * m.m[3][2]);

	det -= (m.m[0][1] * m.m[1][0] * m.m[2][2] * m.m[3][3]);
	det -= (m.m[0][2] * m.m[1][0] * m.m[2][3] * m.m[3][1]);
	det -= (m.m[0][3] * m.m[1][0] * m.m[2][1] * m.m[3][2]);

	det += (m.m[0][3] * m.m[1][0] * m.m[2][2] * m.m[3][1]);
	det += (m.m[0][2] * m.m[1][0] * m.m[2][1] * m.m[3][3]);
	det += (m.m[0][1] * m.m[1][0] * m.m[2][3] * m.m[3][2]);

	det += (m.m[0][1] * m.m[1][2] * m.m[2][0] * m.m[3][3]);
	det += (m.m[0][2] * m.m[1][3] * m.m[2][0] * m.m[3][1]);
	det += (m.m[0][3] * m.m[1][1] * m.m[2][0] * m.m[3][2]);

	det -= (m.m[0][3] * m.m[1][2] * m.m[2][0] * m.m[3][1]);
	det -= (m.m[0][2] * m.m[1][1] * m.m[2][0] * m.m[3][3]);
	det -= (m.m[0][1] * m.m[1][3] * m.m[2][0] * m.m[3][2]);

	det -= (m.m[0][1] * m.m[1][2] * m.m[2][3] * m.m[3][0]);
	det -= (m.m[0][2] * m.m[1][3] * m.m[2][1] * m.m[3][0]);
	det -= (m.m[0][3] * m.m[1][1] * m.m[2][2] * m.m[3][0]);

	det += (m.m[0][3] * m.m[1][2] * m.m[2][1] * m.m[3][0]);
	det += (m.m[0][2] * m.m[1][1] * m.m[2][3] * m.m[3][0]);
	det += (m.m[0][1] * m.m[1][3] * m.m[2][2] * m.m[3][0]);

	if (det != 0) {
		{
			result.m[0][0] =
				(1.0f / det) *
				(m.m[1][1] * m.m[2][2] * m.m[3][3] + m.m[1][2] * m.m[2][3] * m.m[3][1] +
					m.m[1][3] * m.m[2][1] * m.m[3][2] - m.m[1][3] * m.m[2][2] * m.m[3][1] -
					m.m[1][2] * m.m[2][1] * m.m[3][3] - m.m[1][1] * m.m[2][3] * m.m[3][2]);
			result.m[0][1] =
				(1.0f / det) *
				(-m.m[0][1] * m.m[2][2] * m.m[3][3] - m.m[0][2] * m.m[2][3] * m.m[3][1] -
					m.m[0][3] * m.m[2][1] * m.m[3][2] + m.m[0][3] * m.m[2][2] * m.m[3][1] +
					m.m[0][2] * m.m[2][1] * m.m[3][3] + m.m[0][1] * m.m[2][3] * m.m[3][2]);
			result.m[0][2] =
				(1.0f / det) *
				(+m.m[0][1] * m.m[1][2] * m.m[3][3] + m.m[0][2] * m.m[1][3] * m.m[3][1] +
					m.m[0][3] * m.m[1][1] * m.m[3][2] - m.m[0][3] * m.m[1][2] * m.m[3][1] -
					m.m[0][2] * m.m[1][1] * m.m[3][3] - m.m[0][1] * m.m[1][3] * m.m[3][2]);
			result.m[0][3] =
				(1.0f / det) *
				(-m.m[0][1] * m.m[1][2] * m.m[2][3] - m.m[0][2] * m.m[1][3] * m.m[2][1] -
					m.m[0][3] * m.m[1][1] * m.m[2][2] + m.m[0][3] * m.m[1][2] * m.m[2][1] +
					m.m[0][2] * m.m[1][1] * m.m[2][3] + m.m[0][1] * m.m[1][3] * m.m[2][2]);
		}

		{
			result.m[1][0] =
				(1.0f / det) *
				(-m.m[1][0] * m.m[2][2] * m.m[3][3] - m.m[1][2] * m.m[2][3] * m.m[3][0] -
					m.m[1][3] * m.m[2][0] * m.m[3][2] + m.m[1][3] * m.m[2][2] * m.m[3][0] +
					m.m[1][2] * m.m[2][0] * m.m[3][3] + m.m[1][0] * m.m[2][3] * m.m[3][2]);
			result.m[1][1] =
				(1.0f / det) *
				(m.m[0][0] * m.m[2][2] * m.m[3][3] + m.m[0][2] * m.m[2][3] * m.m[3][0] +
					m.m[0][3] * m.m[2][0] * m.m[3][2] - m.m[0][3] * m.m[2][2] * m.m[3][0] -
					m.m[0][2] * m.m[2][0] * m.m[3][3] - m.m[0][0] * m.m[2][3] * m.m[3][2]);
			result.m[1][2] =
				(1.0f / det) *
				(-m.m[0][0] * m.m[1][2] * m.m[3][3] - m.m[0][2] * m.m[1][3] * m.m[3][0] -
					m.m[0][3] * m.m[1][0] * m.m[3][2] + m.m[0][3] * m.m[1][2] * m.m[3][0] +
					m.m[0][2] * m.m[1][0] * m.m[3][3] + m.m[0][0] * m.m[1][3] * m.m[3][2]);
			result.m[1][3] =
				(1.0f / det) *
				(m.m[0][0] * m.m[1][2] * m.m[2][3] + m.m[0][2] * m.m[1][3] * m.m[2][0] +
					m.m[0][3] * m.m[1][0] * m.m[2][2] - m.m[0][3] * m.m[1][2] * m.m[2][0] -
					m.m[0][2] * m.m[1][0] * m.m[2][3] - m.m[0][0] * m.m[1][3] * m.m[2][2]);
		}

		{
			result.m[2][0] =
				(1.0f / det) *
				(m.m[1][0] * m.m[2][1] * m.m[3][3] + m.m[1][1] * m.m[2][3] * m.m[3][0] +
					m.m[1][3] * m.m[2][0] * m.m[3][1] - m.m[1][3] * m.m[2][1] * m.m[3][0] -
					m.m[1][1] * m.m[2][0] * m.m[3][3] - m.m[1][0] * m.m[2][3] * m.m[3][1]);
			result.m[2][1] =
				(1.0f / det) *
				(-m.m[0][0] * m.m[2][1] * m.m[3][3] - m.m[0][1] * m.m[2][3] * m.m[3][0] -
					m.m[0][3] * m.m[2][0] * m.m[3][1] + m.m[0][3] * m.m[2][1] * m.m[3][0] +
					m.m[0][1] * m.m[2][0] * m.m[3][3] + m.m[0][0] * m.m[2][3] * m.m[3][1]);
			result.m[2][2] =
				(1.0f / det) *
				(m.m[0][0] * m.m[1][1] * m.m[3][3] + m.m[0][1] * m.m[1][3] * m.m[3][0] +
					m.m[0][3] * m.m[1][0] * m.m[3][1] - m.m[0][3] * m.m[1][1] * m.m[3][0] -
					m.m[0][1] * m.m[1][0] * m.m[3][3] - m.m[0][0] * m.m[1][3] * m.m[3][1]);
			result.m[2][3] =
				(1.0f / det) *
				(-m.m[0][0] * m.m[1][1] * m.m[2][3] - m.m[0][1] * m.m[1][3] * m.m[2][0] -
					m.m[0][3] * m.m[1][0] * m.m[2][1] + m.m[0][3] * m.m[1][1] * m.m[2][0] +
					m.m[0][1] * m.m[1][0] * m.m[2][3] + m.m[0][0] * m.m[1][3] * m.m[2][1]);
		}

		result.m[3][0] =
			(1.0f / det) * (-m.m[1][0] * m.m[2][1] * m.m[3][2] - m.m[1][1] * m.m[2][2] * m.m[3][0] -
				m.m[1][2] * m.m[2][0] * m.m[3][1] + m.m[1][2] * m.m[2][1] * m.m[3][0] +
				m.m[1][1] * m.m[2][0] * m.m[3][2] + m.m[1][0] * m.m[2][2] * m.m[3][1]);
		result.m[3][1] =
			(1.0f / det) * (m.m[0][0] * m.m[2][1] * m.m[3][2] + m.m[0][1] * m.m[2][2] * m.m[3][0] +
				m.m[0][2] * m.m[2][0] * m.m[3][1] - m.m[0][2] * m.m[2][1] * m.m[3][0] -
				m.m[0][1] * m.m[2][0] * m.m[3][2] - m.m[0][0] * m.m[2][2] * m.m[3][1]);

		result.m[3][2] =
			(1.0f / det) * (-m.m[0][0] * m.m[1][1] * m.m[3][2] - m.m[0][1] * m.m[1][2] * m.m[3][0] -
				m.m[0][2] * m.m[1][0] * m.m[3][1] + m.m[0][2] * m.m[1][1] * m.m[3][0] +
				m.m[0][1] * m.m[1][0] * m.m[3][2] + m.m[0][0] * m.m[1][2] * m.m[3][1]);
		result.m[3][3] =
			(1.0f / det) * (m.m[0][0] * m.m[1][1] * m.m[2][2] + m.m[0][1] * m.m[1][2] * m.m[2][0] +
				m.m[0][2] * m.m[1][0] * m.m[2][1] - m.m[0][2] * m.m[1][1] * m.m[2][0] -
				m.m[0][1] * m.m[1][0] * m.m[2][2] - m.m[0][0] * m.m[1][2] * m.m[2][1]);
	}
	return result;
}

Matrix4x4 MatLib::MakeIdentity4x4() {
	Matrix4x4 result = {};
	for (int y = 0; y < 4; y++) {
		for (int x = 0; x < 4; x++) {
			if (x == y) {
				result.m[y][x] = 1;
			}
		}
	}
	return result;
}

Matrix4x4 MatLib::MakeScaleMatrix(const Vector3& translate) {
	Matrix4x4 result = {};

	result.m[0][0] = translate.x;
	result.m[1][1] = translate.y;
	result.m[2][2] = translate.z;
	result.m[3][3] = 1;

	return result;
}

Matrix4x4 MatLib::MakeTranslateMatrix(const Vector3& translate) {
	Matrix4x4 result = {};
	result.m[0][0] = 1;
	result.m[1][1] = 1;
	result.m[2][2] = 1;
	result.m[3][3] = 1;

	result.m[3][0] = translate.x;
	result.m[3][1] = translate.y;
	result.m[3][2] = translate.z;

	return result;
}

Matrix4x4
MatLib::MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip) {
	Matrix4x4 result = {};

	result.m[0][0] = (1.0f / aspectRatio) * (1.0f / tanf(fovY / 2));
	result.m[1][1] = (1.0f / tanf(fovY / 2));
	result.m[2][2] = (farClip) / (farClip - nearClip);
	result.m[2][3] = 1;
	result.m[3][2] = (-nearClip * farClip) / (farClip - nearClip);

	return result;
}

Matrix4x4 MatLib::MakeOrthographicMatrix(
	float left, float top, float right, float bottom, float nearClip, float farClip) {
	Matrix4x4 result = {};
	result.m[0][0] = 2 / (right - left);
	result.m[1][1] = 2 / (top - bottom);
	result.m[2][2] = 1 / (farClip - nearClip);
	result.m[3][0] = (left + right) / (left - right);
	result.m[3][1] = (top + bottom) / (bottom - top);
	result.m[3][2] = nearClip / (nearClip - farClip);
	result.m[3][3] = 1;
	return result;
}

Matrix4x4 MatLib::MakeViewportMatrix(
	float left, float top, float width, float height, float minDepth, float maxDepth) {
	Matrix4x4 result = {};
	result.m[0][0] = width / 2;
	result.m[1][1] = -height / 2;
	result.m[2][2] = maxDepth - minDepth;
	result.m[3][0] = left + (width / 2);
	result.m[3][1] = top + (height / 2);
	result.m[3][2] = minDepth;
	result.m[3][3] = 1;
	return result;
}

Vector3 MatLib::Transform(const Vector3& vector, const Matrix4x4& matrix) {
	Vector3 result;
	result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0] +
		1.0f * matrix.m[3][0];
	result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1] +
		1.0f * matrix.m[3][1];
	result.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2] +
		1.0f * matrix.m[3][2];
	float w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] + vector.z * matrix.m[2][3] +
		1.0f * matrix.m[3][3];
	assert(w != 0.0f);
	result.x /= w;
	result.y /= w;
	result.z /= w;
	return result;
}

Vector3 MatLib::GetWorldPosition(const Matrix4x4& matWorld) {
	Vector3 worldPos;

	worldPos.x = matWorld.m[3][0];
	worldPos.y = matWorld.m[3][1];
	worldPos.z = matWorld.m[3][2];

	return worldPos;
}

Vector3 MatLib::TransformNormal(const Vector3& v, const Matrix4x4& mat)
{
	Vector3 result{
		v.x * mat.m[0][0] + v.y * mat.m[1][0] + v.z * mat.m[2][0],
		v.x * mat.m[0][1] + v.y * mat.m[1][1] + v.z * mat.m[2][1],
		v.x * mat.m[0][2] + v.y * mat.m[1][2] + v.z * mat.m[2][2] };
	return result;
}

Matrix4x4 MatLib::MakeBillBoard(const Vector3& target, const Vector3& eye, const Vector3& up)
{
	// 回転行列
	// X軸
	Vector3 zAxis = MathCalc::Normalize(VectorLib::Subtract(target, eye));
	// Y軸
	Vector3 xAxis = MathCalc::Normalize(MathCalc::Cross(up, zAxis));
	// Z軸
	Vector3 yAxis = MathCalc::Cross(zAxis, xAxis);

	return {
		xAxis.x,xAxis.y,xAxis.z,0.0f,
		yAxis.x,yAxis.y,yAxis.z,0.0f,
		zAxis.x,zAxis.y,zAxis.z,0.0f,
		0.0f,0.0f,0.0f,1.0f
	};
}

Matrix4x4 MatLib::DirectionToDirection(const Vector3& from, const Vector3& to)
{
	// cos
	float cosine = MathCalc::Dot(from, to);
	// sin
	float sine = MathCalc::Length(MathCalc::Cross(from, to));
	// normalize
	Vector3 normal = MathCalc::Normalize(MathCalc::Cross(from, to));

	Matrix4x4 result = MakeIdentity4x4();
	result.m[0][0] = std::powf(normal.x, 2) * (1 - cosine) + cosine;
	result.m[0][1] = (normal.x * normal.y) * (1 - cosine) + (normal.z * sine);
	result.m[0][2] = (normal.x * normal.z) * (1 - cosine) - (normal.y * sine);

	result.m[1][0] = (normal.x * normal.y) * (1 - cosine) - (normal.z * sine);
	result.m[1][1] = std::powf(normal.y, 2) * (1 - cosine) + cosine;
	result.m[1][2] = (normal.y * normal.z) * (1 - cosine) + normal.x * sine;

	result.m[2][0] = (normal.x * normal.z) * (1 - cosine) + (normal.y * sine);
	result.m[2][1] = (normal.y * normal.z) * (1 - cosine) - (normal.x * sine);
	result.m[2][2] = std::powf(normal.z, 2) * (1 - cosine) + cosine;

	return result;
}

Matrix4x4 MatLib::MakeRotateAxisAngle(const Vector3& axis, float angle)
{
	axis, angle;
	return Matrix4x4();
}

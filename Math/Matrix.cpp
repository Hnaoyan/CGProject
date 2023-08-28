#include "Matrix.h"
#include <cmath>

Matrix4x4 MakeAffineMatrix(
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
Matrix4x4 MakeRotateXMatrix(float radian) {
	Matrix4x4 result = {};
	result.m[0][0] = 1.0f;
	result.m[1][1] = std::cosf(radian);
	result.m[1][2] = std::sinf(radian);
	result.m[2][1] = std::sinf(-radian);
	result.m[2][2] = std::cosf(radian);
	result.m[3][3] = 1.0f;
	return result;
}

Matrix4x4 MakeRotateYMatrix(float radian) {
	Matrix4x4 result = {};
	result.m[0][0] = std::cosf(radian);
	result.m[0][2] = std::sinf(-radian);
	result.m[1][1] = 1.0f;
	result.m[2][0] = std::sinf(radian);
	result.m[2][2] = std::cosf(radian);
	result.m[3][3] = 1.0f;
	return result;
}

Matrix4x4 MakeRotateZMatrix(float radian) {
	Matrix4x4 result = {};
	result.m[0][0] = std::cosf(radian);
	result.m[0][1] = std::sinf(radian);
	result.m[1][0] = std::sinf(-radian);
	result.m[1][1] = std::cosf(radian);
	result.m[2][2] = 1.0f;
	result.m[3][3] = 1.0f;
	return result;
}

Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2) {
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


Matrix4x4 MakeInverse(const Matrix4x4& m) {
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

Matrix4x4 MakeIdentity4x4() {
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

Matrix4x4 MakeScaleMatrix(const Vector3& translate) {
	Matrix4x4 result = {};

	result.m[0][0] = translate.x;
	result.m[1][1] = translate.y;
	result.m[2][2] = translate.z;
	result.m[3][3] = 1;

	return result;
}

Matrix4x4 MakeTranslateMatrix(const Vector3& translate) {
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

Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip) {
	Matrix4x4 result = {};

	result.m[0][0] = (1.0f / aspectRatio) * (1.0f / tanf(fovY / 2));
	result.m[1][1] = (1.0f / tanf(fovY / 2));
	result.m[2][2] = (farClip) / (farClip - nearClip);
	result.m[2][3] = 1;
	result.m[3][2] = (-nearClip * farClip) / (farClip - nearClip);

	return result;
}

Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip) {
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

Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth) {
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

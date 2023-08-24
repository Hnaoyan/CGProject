#include "Camera.h"

//Matrix4x4 Camera::SetViewProjectionMat(const Vector3& translate, const Vector3& scale, const Vector3& rotate, float fovY, float nearClip, float farClip)
//{
//	Matrix4x4 worldMatrix = MakeAffineMatrix(transf.scale, transform.rotate, transform.translate);
//	// カメラから描画まで
//	Matrix4x4 cameraMatrix = MakeAffineMatrix(camera.scale, camera.rotate, camera.translate);
//	Matrix4x4 viewMatrix = MakeInverse(cameraMatrix);
//	Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(fovY, float(WinApp::kClientWidth) / float(WinApp::kClientHeight), nearClip, farClip);
//	Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));
//
//    return worldViewProjectionMatrix;
//}

Matrix4x4 Camera::SetViewProjectionMat(const Vector3& translate,const Vector3& scale,const Vector3& rotate, float nearClip, float farClip)
{
	Matrix4x4 worldSpriteMatrix = MakeAffineMatrix(scale, rotate,translate);
	Matrix4x4 viewMatrixSprite = MakeIdentity4x4();
	Matrix4x4 projectionMatrixSprite = MakeOrthographicMatrix(0.0f, 0.0f, float(WinApp::kClientWidth), float(WinApp::kClientHeight), nearClip, farClip);
	Matrix4x4 worldViewProjectionMatrixSprite = Multiply(worldSpriteMatrix, Multiply(viewMatrixSprite, projectionMatrixSprite));

	return worldViewProjectionMatrixSprite;
}

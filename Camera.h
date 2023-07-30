#pragma once
#include "WinApp.h"
#include "StructManager.h"
#include "Sprite.h"

class Camera
{
public:
	/// <summary>
	/// 透視投影行列による変換(3D用)
	/// </summary>
	/// <param name="transform"></param>
	/// <param name="camera"></param>
	/// <param name="fovY"></param>
	/// <param name="nearClip"></param>
	/// <param name="farClip"></param>
	/// <returns></returns>
	static Matrix4x4 SetViewProjectionMat(const Sprite::Transform transform, const Sprite::Transform camera, float fovY, float nearClip, float farClip);

	/// <summary>
	/// 正射影行列による変換(2D用)
	/// </summary>
	/// <param name="transform"></param>
	/// <param name="nearClip"></param>
	/// <param name="farClip"></param>
	/// <returns></returns>
	static Matrix4x4 SetViewProjectionMat(const Sprite::Transform transform, float nearClip, float farClip);

};


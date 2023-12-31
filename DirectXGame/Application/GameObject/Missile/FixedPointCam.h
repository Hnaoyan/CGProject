#pragma once
#include "Camera/ICamera.h"
#include "StructManager.h"

class FixedPointCam : public ICamera
{
public:
	void Initialize() override;
	void Update() override;
	void ImGuiWidget() override;

private:
	void RotationUpdate();

private:

	Vector3 defaultPoint_;
	Vector3 offset_;

};


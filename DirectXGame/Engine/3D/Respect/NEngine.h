#pragma once
#include "DirectXCommon.h"

class NEngine
{
public:
	void Initialize(DirectXCommon* dxCommon) {
		dxCommon_ = dxCommon;
	}

	void CreateRootSignatureP();
	void CreateInputlayoutP();
	void SettingRasterrizerP();
	void InitPSOP();

private:
	DirectXCommon* dxCommon_ = nullptr;

};


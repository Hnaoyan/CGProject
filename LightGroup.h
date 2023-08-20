#pragma once
#include "StructManager.h"
#include "DirectionalLight.h"

#include <wrl.h>
#include <dxgidebug.h>
#include <dxcapi.h>
#pragma comment(lib, "dxcompiler.lib")

#include<d3d12.h>
#include<dxgi1_6.h>

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

class LightGroup
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public:

	static const int kDirLightNum = 3;
	static const int kPointLightNum = 3;
	static const int kSpotLightNum = 3;
	static const int kCircleShadowNum = 1;

public:

	struct ConstBufferData
	{
		Vector3 ambientColor;
		float pad1;
		// 平行光源
		DirectionalLight::ConstBufferData dirLights[kDirLightNum];

	};

public:
	static LightGroup* Create();

private:
	// 定数バッファ
	ComPtr<ID3D12Resource> constBuff_;

	ConstBufferData* constMap_ = nullptr;

	Vector3 ambientColor_ = { 1,1,1 };

	DirectionalLight dirLights_[kDirLightNum];

	bool dirty_ = false;
	

public:	// メンバ関数

	void Initialize();

	void Update();

	void Draw(ID3D12GraphicsCommandList* cmdList, UINT rootParameterIndex);

	void TransferConstBuffer();

	void DefaultLightSetting();

public:	// セット

	void SetAmbientColor(const Vector3& color);

	void SetDirLightActive(int index, bool active);

	void SetDirLightDir(int index, const Vector4& lightdir);

	void SetDirLightColor(int index, const Vector3& lightcolor);

};


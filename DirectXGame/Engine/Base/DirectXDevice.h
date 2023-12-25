#pragma once
#include <wrl.h>

// DirectX
#include <d3d12.h>
#include <dxgi1_6.h>
#include <dxcapi.h>
#pragma comment(lib, "dxcompiler.lib")

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

class DirectXDevice
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	static DirectXDevice* GetInstance() {
		static DirectXDevice instance;
		return &instance;
	}

public: // アクセッサ
	IDXGIFactory7* GetFactory() { return dxgiFactory_.Get(); }
	ID3D12Device* GetDevice() { return device_.Get(); }

public:

	Microsoft::WRL::ComPtr<IDXGIFactory7> dxgiFactory_;
	Microsoft::WRL::ComPtr<ID3D12Device> device_;

};


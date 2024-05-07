#pragma once
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXTex.h>

class SRV;

class RenderSprite {
public:
	static RenderSprite* GetInstance() {
		static RenderSprite instance;
		return &instance;
	}

	void Initialize(ID3D12Device* device, ID3D12GraphicsCommandList* commandList);

	void Draw(SRV* srv);

private:
	ID3D12Device* device_ = nullptr;
	ID3D12GraphicsCommandList* commandList_ = nullptr;

};
#pragma once
#include<d3d12.h>
#include<dxgi1_6.h>
#include<cassert>

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

class DirectX
{
public:
	//初期化
	void Initialize();

	void DXGIDevice();

};


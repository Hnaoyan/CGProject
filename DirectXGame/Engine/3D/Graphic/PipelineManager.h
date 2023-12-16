#pragma once

#include <dxgidebug.h>
#include <dxcapi.h>
#pragma comment(lib, "dxcompiler.lib")

#include<d3d12.h>
#include<dxgi1_6.h>

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

namespace Pipeline 
{
	enum class RootParameter : int {
		kWorldTransform,
		kViewProjection,
		kMaterial,
		kTexture,
		kLight,
		kCountOfParameter,	// サイズを取得する為の値
	};

	enum class BlendMode : int {
		kNone,
		kNormal,
		kAdd,
		kSubtract,
		kMultiply,
		kScreen,
		kCountOfBlendMode,
	};

}

class PipelineManager
{
};


#include "Model.h"

using namespace Microsoft::WRL;

void Model::StaticInitialize()
{

	// パイプライン初期化
	InitializeGraphicsPipeline();

}

void Model::InitializeGraphicsPipeline()
{
	HRESULT result = S_FALSE;
	ComPtr<ID3DBlob> vsBlob;
	ComPtr<ID3DBlob> psBlob;
	ComPtr<ID3DBlob> errorBlob;

}

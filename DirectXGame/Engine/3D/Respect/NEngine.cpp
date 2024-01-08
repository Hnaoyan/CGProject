#include "NEngine.h"

void NEngine::CreateRootSignatureP()
{
	//RootSignature作成
	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};
	descriptionRootSignature.Flags =
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	//RootParameter作成。複数設定できるので配列。
	D3D12_ROOT_PARAMETER rootParameters[5] = {};
	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;//CBVを使う
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;//pixelShaderを使う
	rootParameters[0].Descriptor.ShaderRegister = 0;//レジスタ番号0とバインド
	//worldtransform
	D3D12_DESCRIPTOR_RANGE descriptoraRangeParticle[1] = {};
	descriptoraRangeParticle[0].BaseShaderRegister = 0;
	descriptoraRangeParticle[0].NumDescriptors = 1;
	descriptoraRangeParticle[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;//SRVを使用
	descriptoraRangeParticle[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;//Offsetを自動計算
	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;//CBVを使う
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;//vertexShaderを使う
	rootParameters[1].DescriptorTable.pDescriptorRanges = descriptoraRangeParticle;//レジスタ番号0とバインド
	rootParameters[1].DescriptorTable.NumDescriptorRanges = _countof(descriptoraRangeParticle);
	//viewProjection
	rootParameters[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;//CBVを使う
	rootParameters[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;//vertexShaderで使う
	rootParameters[4].Descriptor.ShaderRegister = 1;//レジスタ番号を1にバインド

	D3D12_DESCRIPTOR_RANGE descriptoraRange[1] = {};
	descriptoraRange[0].BaseShaderRegister = 1;
	descriptoraRange[0].NumDescriptors = 1;
	descriptoraRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;//SRVを使用
	descriptoraRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;//Offsetを自動計算
	rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;//Descriptortableを使う
	rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;//PixcelShaderを使う
	rootParameters[2].DescriptorTable.pDescriptorRanges = descriptoraRange;//tableの中身の配列を指定
	rootParameters[2].DescriptorTable.NumDescriptorRanges = _countof(descriptoraRange);

	rootParameters[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;//CBVを使う
	rootParameters[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;//pixcelShaderを使う
	rootParameters[3].Descriptor.ShaderRegister = 1;//レジスタ番号1

	D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};
	staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;//バイリニアフィルタ
	staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;//０～１の範囲外をリピート
	staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;//比較しない
	staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX;//ありったけのmipmapを使う
	staticSamplers[0].ShaderRegister = 0;
	staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;




	descriptionRootSignature.pParameters = rootParameters;//ルートパラメータ配列へのポインタ
	descriptionRootSignature.NumParameters = _countof(rootParameters);
	descriptionRootSignature.pStaticSamplers = staticSamplers;
	descriptionRootSignature.NumStaticSamplers = _countof(staticSamplers);
	//シリアライズしてバイナリにする

	HRESULT hr;
	hr = D3D12SerializeRootSignature(&descriptionRootSignature,
		D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlobParticle_, &errorBlobParticle_);
	if (FAILED(dxCommon_->GetHr())) {
		Log(reinterpret_cast<char*>(errorBlobParticle_->GetBufferPointer()));
		assert(false);
	}
	//バイナリを元に生成

	hr = dxCommon_->GetDevice()->CreateRootSignature(0, signatureBlobParticle_->GetBufferPointer(),
		signatureBlobParticle_->GetBufferSize(), IID_PPV_ARGS(&rootSignatureParticle_));
	assert(SUCCEEDED(hr));
}

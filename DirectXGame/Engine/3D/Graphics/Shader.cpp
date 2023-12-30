#include "Shader.h"
#include "WindowAPI.h"
#include <cassert>

using namespace Microsoft::WRL;

ComPtr<IDxcUtils> Shader::dxcUtils_;
ComPtr<IDxcCompiler3> Shader::dxcCompiler_;
ComPtr<IDxcIncludeHandler> Shader::includeHandler_;

const std::wstring Shader::kBaseDirectory = L"Resources/shaders/";

void Shader::Initialize()
{

	HRESULT result = S_FALSE;
	result = DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&dxcUtils_));

	assert(SUCCEEDED(result));
	result = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&dxcCompiler_));
	assert(SUCCEEDED(result));

	result = dxcUtils_->CreateDefaultIncludeHandler(includeHandler_.GetAddressOf());
	assert(SUCCEEDED(result));

}

IDxcBlob* Shader::Compile(const std::wstring& filePath, const wchar_t* profile)
{
	// フルパス
	std::wstring fullPath = Shader::kBaseDirectory + filePath;
	// ここの中身をこの後書く
		// 1.hlslファイルを読む
		// これからシェーダーをコンパイルする旨をログに出す
	Log(ConvertString(std::format(L"Begin CompileShader, path:{}, profile:{}\n", fullPath, profile)));
	// hlslファイルを読む
	IDxcBlobEncoding* shaderSource = nullptr;
	HRESULT hr = dxcUtils_->LoadFile(fullPath.c_str(), nullptr, &shaderSource);
	// 読めなかったら止める
	assert(SUCCEEDED(hr));
	// 読み込んだファイルの内容を設定する
	DxcBuffer shaderSourceBuffer;
	shaderSourceBuffer.Ptr = shaderSource->GetBufferPointer();
	shaderSourceBuffer.Size = shaderSource->GetBufferSize();
	shaderSourceBuffer.Encoding = DXC_CP_UTF8;// UTF8の文字コードであることを通知
	// 2.Compileする
	LPCWSTR arguments[] = {
		fullPath.c_str(),	// コンパイル対象のhlslファイル名
		L"-E", L"main",	// エントリーポイントの指定。基本的にmain以外にはしない
		L"-T", profile,	// ShaderProfileの設定
		L"-Zi", L"-Qembed_debug",	// デバッグ用の情報を埋め込む
		L"-Od",	// 最適化を外しておく
		L"-Zpr",	// メモリレイアウトは行優先
	};
	// 実際にShaderをコンパイルする
	IDxcResult* shaderResult = nullptr;
	hr = dxcCompiler_->Compile(
		&shaderSourceBuffer,	// 読み込んだファイル
		arguments,				// コンパイルオプション
		_countof(arguments),	// コンパイルオプションの数
		includeHandler_.Get(),			// includeが含まれた諸々
		IID_PPV_ARGS(&shaderResult)	// コンパイル結果
	);
	// コンパイルエラーではなくdxcが起動できないなど致命的な状況
	assert(SUCCEEDED(hr));
	// 3.警告・エラーを確認する
	// 警告・エラーが出てたらログに出して止める
	IDxcBlobUtf8* shaderError = nullptr;
	shaderResult->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&shaderError), nullptr);
	if (shaderError != nullptr && shaderError->GetStringLength() != 0) {
		Log(shaderError->GetStringPointer());
		// 警告・エラーダメゼッタイ
		assert(false);
	}
	// 4.Compile結果を受け取って返す
	IDxcBlob* shaderBlob = nullptr;
	hr = shaderResult->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&shaderBlob), nullptr);
	assert(SUCCEEDED(hr));
	// 成功したログを出す
	Log(ConvertString(std::format(L"Compile Succeeded, path:{}, profile:{}\n", fullPath, profile)));
	// もう使わないリソースを解放
	shaderSource->Release();
	shaderResult->Release();
	// 実行用のバイナリを返却
	return shaderBlob;
}

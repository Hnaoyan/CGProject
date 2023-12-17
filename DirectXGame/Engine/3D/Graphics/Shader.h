#pragma once
#include "StringManager.h"
#include <wrl.h>

#include <dxgidebug.h>
#include <dxcapi.h>
#pragma comment(lib, "dxcompiler.lib")

#include<d3d12.h>
#include<dxgi1_6.h>

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")


class Shader
{
public:
	static Shader* GetInstance() {
		static Shader instance;
		return &instance;
	}

private: // Handler
	static Microsoft::WRL::ComPtr<IDxcIncludeHandler> includeHandler_;

	static Microsoft::WRL::ComPtr<IDxcUtils> dxcUtils_;

	static Microsoft::WRL::ComPtr<IDxcCompiler3> dxcCompiler_;

public:
	/// <summary>
	/// DXCの初期化
	/// </summary>
	static void Initialize();

	/// <summary>
	/// Shaderのコンパイル
	/// </summary>
	/// <param name="filePath"></param>
	/// <param name="profile"></param>
	/// <returns></returns>
	static IDxcBlob* Compile(const std::wstring& filePath, const wchar_t* profile);

private:
	// ファイルまでのパス
	static const std::wstring kBaseDirectory;

};


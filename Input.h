#pragma once
#include <wrl.h>
#include <array>

#include <Xinput.h>
#define DIRECTINPUT_VERSION		0x0800	// DirectInputのバージョン指定
#include <dinput.h>

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

class Input
{
public:	// メンバ関数
	/// <summary>
	/// シングルトンインスタンス
	/// </summary>
	/// <returns></returns>
	static Input* GetInstance();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 押した瞬間True
	/// </summary>
	/// <param name="keyNumber"></param>
	/// <returns></returns>
	bool TrigerKey(BYTE keyNumber) const;
	
	/// <summary>
	/// 押している間True
	/// </summary>
	/// <param name="keyNumber"></param>
	/// <returns></returns>
	bool PressKey(BYTE keyNumber) const;

	/// <summary>
	/// 離した瞬間True
	/// </summary>
	/// <param name="keyNumber"></param>
	/// <returns></returns>
	bool ReleaseKey(BYTE keyNumber) const;

private:
	Microsoft::WRL::ComPtr<IDirectInput8> directInput_;
	Microsoft::WRL::ComPtr<IDirectInputDevice8> dKeyBoard_;
	Microsoft::WRL::ComPtr<IDirectInputDevice8> dMouse_;
	
	std::array<BYTE, 256> key_;
	std::array<BYTE, 256> preKey_;

	HWND hwnd_;

};


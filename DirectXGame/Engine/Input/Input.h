#pragma once
#include <wrl.h>
#include <array>
#include <vector>

#include "StructManager.h"

#include <Xinput.h>
#define DIRECTINPUT_VERSION		0x0800	// DirectInputのバージョン指定
#include <dinput.h>

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

class Input
{
private:
	// ComPtrのテンプレート
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public: // インナークラス
	struct MouseMove {
		LONG lX;
		LONG lY;
		LONG lZ;
	};

public:

	enum class PadType {
		DirectInput,
		XInput,
	};

	union State {
		XINPUT_STATE xInput_;
		DIJOYSTATE2 directInput_;
	};

	struct JoyStick {
		ComPtr<IDirectInputDevice8> device_;
		int32_t deadZoneL_;
		int32_t deadZoneR_;
		PadType type_;
		State state_;
		State statePre_;
	};


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
	bool TriggerKey(BYTE keyNumber) const;
	
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

public:	// コントローラー系
	
	/// <summary>
	/// 
	/// </summary>
	/// <param name="stickNo"></param>
	/// <param name="out"></param>
	/// <returns></returns>
	bool GetJoystickState(int32_t stickNo, DIJOYSTATE2& out);

	bool GetJoystickStatePrevious(int32_t stickNo, DIJOYSTATE2& out);

	bool GetJoystickState(int32_t stickNo, XINPUT_STATE& out);

	bool GetJoystickStatePrevious(int32_t stickNo, XINPUT_STATE& out);

	void SetJoystickDeadZone(int32_t stickNo, int32_t deadZoneL, int32_t deadZoneR);

	size_t GetNumberOfJoysticks();

public: // マウス系

	const std::array<BYTE, 256>& GetAllKey() const { return key_; }

	bool IsPressMouse(int32_t buttonNumber);

	bool IsTriggerMouse(int32_t buttonNumber);

	/// <summary>
	/// マウスの移動量
	/// </summary>
	/// <returns></returns>
	MouseMove GetMouseMove();

	/// <summary>
	/// マウスの座標
	/// </summary>
	/// <returns></returns>
	const Vector2& GetMousePosition() const;

private:
	static BOOL CALLBACK EnumJoysticksCallback(const DIDEVICEINSTANCE* pdidInstance, VOID* pContext) noexcept;

	Input() = default;
	~Input();
	Input(const Input&) = delete;
	const Input& operator=(const Input&) = delete;
	// ジョイスティックの設定
	void SetupJoySticks();

private:

	ComPtr<IDirectInput8> directInput_;
	ComPtr<IDirectInputDevice8> dKeyBoard_;
	ComPtr<IDirectInputDevice8> dMouse_;
	
	std::vector<JoyStick> devJoysticks_;
	std::array<BYTE, 256> key_;
	std::array<BYTE, 256> preKey_;

	DIMOUSESTATE2 mouse_;
	DIMOUSESTATE2 preMouse_;

	HWND hwnd_;

	Vector2 mousePosition_;
};


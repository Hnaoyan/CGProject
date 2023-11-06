#include "Input.h"
#include "WindowAPI.h"
#include <cassert>

#include <XInput.h>
#include <basetsd.h>
#include <commctrl.h>
#include <dbt.h>
#include <oleauto.h>
#include <shellapi.h>
#include <wbemidl.h>
#include <wrl/client.h>

#include <Windows.h>
#include <dinputd.h>
#include <memory>

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "xinput.lib")

#define SAFE_RELEASE(p)                                                                            \
	{                                                                                              \
		if (p) {                                                                                   \
			(p)->Release();                                                                        \
			(p) = nullptr;                                                                         \
		}                                                                                          \
	}

namespace {

	std::vector<DWORD> sXInputVidPids;
	IDirectInputDevice8* sCurrentDevice = nullptr;
	bool sRefreshInputDevices = false;

	bool IsPress(const DIMOUSESTATE2& mouseState, int32_t buttonNumber) {
		assert(0 <= buttonNumber && buttonNumber < _countof(mouseState.rgbButtons));
		return (mouseState.rgbButtons[buttonNumber] & 0x80) != 0;
	}

	HRESULT SetupForIsXInputDevice() noexcept {
		struct bstr_deleter {
			void operator()(void* p) noexcept { SysFreeString(static_cast<BSTR>(p)); }
		};
		using ScopedBSTR = std::unique_ptr<OLECHAR[], bstr_deleter>;

		sXInputVidPids.clear();
		// COM needs to be initialized on this thread before the enumeration.

		// So we can call VariantClear() later, even if we never had a successful
		VARIANT var = {};
		VariantInit(&var);

		// Create WMI
		Microsoft::WRL::ComPtr<IWbemLocator> pIWbemLocator;
		HRESULT hr = CoCreateInstance(
			__uuidof(WbemLocator), nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pIWbemLocator));
		if (FAILED(hr) || pIWbemLocator == nullptr) {
			return hr;
		}

		// Create BSTRs for WMI
		ScopedBSTR bstrNamespace(SysAllocString(L"\\\\.\\root\\cimv2"));
		ScopedBSTR bstrClassName(SysAllocString(L"Win32_PNPEntity"));
		ScopedBSTR bstrDeviceID(SysAllocString(L"DeviceID"));
		if (!bstrNamespace || !bstrClassName || !bstrDeviceID) {
			return E_OUTOFMEMORY;
		}

		// Connect to WMI
		Microsoft::WRL::ComPtr<IWbemServices> pIWbemServices;
		hr = pIWbemLocator->ConnectServer(
			bstrNamespace.get(), nullptr, nullptr, 0L, 0L, nullptr, nullptr, &pIWbemServices);
		if (FAILED(hr) || pIWbemServices == nullptr) {
			return hr;
		}

		// Switch security level to IMPERSONATE
		hr = CoSetProxyBlanket(
			pIWbemServices.Get(), RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, nullptr, RPC_C_AUTHN_LEVEL_CALL,
			RPC_C_IMP_LEVEL_IMPERSONATE, nullptr, EOAC_NONE);
		if (FAILED(hr)) {
			return hr;
		}

		// Get list of Win32_PNPEntity devices
		Microsoft::WRL::ComPtr<IEnumWbemClassObject> pEnumDevices;
		hr = pIWbemServices->CreateInstanceEnum(bstrClassName.get(), 0, nullptr, &pEnumDevices);
		if (FAILED(hr) || pEnumDevices == nullptr) {
			return hr;
		}

		// Loop over all devices
		IWbemClassObject* pDevices[20] = {};
		for (;;) {
			ULONG uReturned = 0;
			hr = pEnumDevices->Next(10000, _countof(pDevices), pDevices, &uReturned);
			if (FAILED(hr)) {
				return hr;
			}

			if (uReturned == 0) {
				break;
			}

			assert(uReturned <= _countof(pDevices));
			_Analysis_assume_(uReturned <= _countof(pDevices));

			for (size_t iDevice = 0; iDevice < uReturned; ++iDevice) {
				if (!pDevices[iDevice]) {
					continue;
				}

				// For each device, get its device ID
				hr = pDevices[iDevice]->Get(bstrDeviceID.get(), 0L, &var, nullptr, nullptr);
				if (SUCCEEDED(hr) && var.vt == VT_BSTR && var.bstrVal != nullptr) {
					// Check if the device ID contains "IG_".  If it does, then it's an XInput device
					// Unfortunately this information can not be found by just using DirectInput
					if (wcsstr(var.bstrVal, L"IG_")) {
						// If it does, then get the VID/PID from var.bstrVal
						DWORD dwPid = 0, dwVid = 0;
						const WCHAR* strVid = wcsstr(var.bstrVal, L"VID_");
						if (strVid && swscanf_s(strVid, L"VID_%4X", &dwVid) != 1) {
							dwVid = 0;
						}
						const WCHAR* strPid = wcsstr(var.bstrVal, L"PID_");
						if (strPid && swscanf_s(strPid, L"PID_%4X", &dwPid) != 1) {
							dwPid = 0;
						}

						const DWORD dwVidPid = MAKELONG(dwVid, dwPid);

						// Add the VID/PID to a linked list
						sXInputVidPids.push_back(dwVidPid);
					}
				}

				VariantClear(&var);
				SAFE_RELEASE(pDevices[iDevice]);
			}
		}

		VariantClear(&var);

		for (size_t iDevice = 0; iDevice < _countof(pDevices); ++iDevice) {
			SAFE_RELEASE(pDevices[iDevice]);
		}

		return hr;
	}

	bool IsXInputDevice(const GUID* pGuidProductFromDirectInput) noexcept {
		for (DWORD vidPid : sXInputVidPids) {
			if (vidPid == pGuidProductFromDirectInput->Data1) {
				return true;
			}
		}
		return false;
	}

	BOOL CALLBACK EnumAxesCallback(const DIDEVICEOBJECTINSTANCE* pdidoi, VOID* pContext) {
		pContext;
		if (!sCurrentDevice) {
			return DIENUM_STOP;
		}

		DIPROPRANGE diprg;
		diprg.diph.dwSize = sizeof(DIPROPRANGE);
		diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER);
		diprg.diph.dwHow = DIPH_BYID;
		diprg.diph.dwObj = pdidoi->dwType;
		diprg.lMin = -32768;
		diprg.lMax = 32767;
		HRESULT hr = sCurrentDevice->SetProperty(DIPROP_RANGE, &diprg.diph);

		if (FAILED(hr)) {
			return DIENUM_STOP;
		}
		return DIENUM_CONTINUE;
	}

	LRESULT SubWndProc(int code, WPARAM wParam, LPARAM lParam) {
		if (code < 0) {
			return CallNextHookEx(NULL, code, wParam, lParam);
		}

		PCWPSTRUCT msg = PCWPSTRUCT(lParam);
		if (msg->message == WM_DEVICECHANGE) {
			switch (msg->wParam) {
			case DBT_DEVICEARRIVAL:
			case DBT_DEVICEREMOVECOMPLETE:
				sRefreshInputDevices = true;
				break;
			}
		}

		return CallNextHookEx(NULL, code, wParam, lParam);
	}

} // namespace

Input* Input::GetInstance()
{
    static Input instance;
    return &instance;
}

Input::~Input() {
    if (dKeyBoard_) {
        dKeyBoard_->Unacquire();
    }
    if (dMouse_) {
        dMouse_->Unacquire();
    }
    for (auto& joyStick : devJoysticks_) {
        joyStick.device_->Unacquire();
    }
}

void Input::Initialize()
{
    WindowAPI* winApp = WindowAPI::GetInstance();
    hwnd_ = winApp->GetHwnd();

    HRESULT result = S_FALSE;
    // DirectInputの初期化
    result = DirectInput8Create(winApp->GetHInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8,
        (void**)&directInput_, nullptr);
    assert(SUCCEEDED(result));

    // キーボードデバイスの生成
    result = directInput_->CreateDevice(GUID_SysKeyboard, &dKeyBoard_, NULL);
    assert(SUCCEEDED(result));
    // 入力データ形式のセット
    result = dKeyBoard_->SetDataFormat(&c_dfDIKeyboard);    // 標準形式
    assert(SUCCEEDED(result));
    // 排他制御レベルのセット
    result = dKeyBoard_->SetCooperativeLevel(winApp->GetHwnd(),
        DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
    assert(SUCCEEDED(result));

    // マウスの生成
    result = directInput_->CreateDevice(GUID_SysMouse, &dMouse_, nullptr);
    assert(SUCCEEDED(result));
    // 入力データ形式のセット
    result = dMouse_->SetDataFormat(&c_dfDIMouse2);
    assert(SUCCEEDED(result));
    // 排他制御レベルのセット
    result = dMouse_->SetCooperativeLevel(winApp->GetHwnd(),
        DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
    assert(SUCCEEDED(result));

	SetupForIsXInputDevice();

	SetupJoySticks();
    

	DEV_BROADCAST_DEVICEINTERFACE notificationFilter{};
	notificationFilter.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
	notificationFilter.dbcc_size = sizeof(notificationFilter);

	HDEVNOTIFY notifyResult;
	notifyResult = RegisterDeviceNotification(
		hwnd_, &notificationFilter,
		DEVICE_NOTIFY_WINDOW_HANDLE | DEVICE_NOTIFY_ALL_INTERFACE_CLASSES);
	assert(!!notifyResult);

	SetWindowsHookExW(
		WH_CALLWNDPROC, (HOOKPROC)&SubWndProc, GetModuleHandleW(NULL), GetCurrentThreadId());

}

void Input::Update()
{
	if (sRefreshInputDevices) {
		// XInputとJoySticksの処理
		SetupForIsXInputDevice();
		SetupJoySticks();
		sRefreshInputDevices = false;
	}

    // キーボード情報の取得開始
    dKeyBoard_->Acquire();
    dMouse_->Acquire();
	for (auto& joyStick : devJoysticks_) {
		joyStick.device_->Acquire();
	}

    preKey_ = key_;

    // キーの入力
    key_.fill(0);
    dKeyBoard_->GetDeviceState(sizeof(key_), key_.data());

    // マウスの入力
    //std::memset(&)
    
	int32_t xInputIndex = 0;
	for (auto& joyStick : devJoysticks_) {
		// 前フレーム
		joyStick.statePre_ = joyStick.state_;
		std::memset(&joyStick.state_, 0, sizeof(joyStick.state_));
		
		if (joyStick.type_ == PadType::DirectInput) {
			auto& directInput = joyStick.state_.directInput_;
			joyStick.device_->GetDeviceState(sizeof(directInput), &directInput);
			if (std::abs(directInput.lX) < joyStick.deadZoneL_) {
				directInput.lX = 0;
			}
			if (std::abs(directInput.lY) < joyStick.deadZoneL_) {
				directInput.lY = 0;
			}
			if (std::abs(directInput.lRx) < joyStick.deadZoneR_) {
				directInput.lRx = 0;
			}
			if (std::abs(directInput.lRy) < joyStick.deadZoneR_) {
				directInput.lRy = 0;
			}
		}
		else {
			XInputGetState(xInputIndex, &joyStick.state_.xInput_);
			auto& gamePad = joyStick.state_.xInput_.Gamepad;
			if (std::abs(gamePad.sThumbLX) < joyStick.deadZoneL_) {
				gamePad.sThumbLX = 0;
			}
			if (std::abs(gamePad.sThumbLY) < joyStick.deadZoneL_) {
				gamePad.sThumbLY = 0;
			}
			if (std::abs(gamePad.sThumbRX) < joyStick.deadZoneR_) {
				gamePad.sThumbRX = 0;
			}
			if (std::abs(gamePad.sThumbRY) < joyStick.deadZoneR_) {
				gamePad.sThumbRY = 0;
			}
		}

	}

	POINT mousePosition;
	// マウス座標
	GetCursorPos(&mousePosition);

	// ウィンドウ座標に変換
	ScreenToClient(hwnd_, &mousePosition);
	mousePosition_.x = static_cast<float>(mousePosition.x);
	mousePosition_.y = static_cast<float>(mousePosition.y);
}

bool Input::TriggerKey(BYTE keyNumber) const
{
    if (key_[keyNumber] && !preKey_[keyNumber]) {
        return true;
    }
    return false;
}

bool Input::PressKey(BYTE keyNumber) const
{
    if (key_[keyNumber]) {
        return true;
    }
    return false;
}

bool Input::ReleaseKey(BYTE keyNumber) const
{
    if (!key_[keyNumber] && preKey_[keyNumber]) {
        return true;
    }
    return false;
}

bool Input::GetJoystickState(int32_t stickNo, DIJOYSTATE2& out)
{
	if (0 <= stickNo && stickNo < devJoysticks_.size()) {
		if (devJoysticks_[stickNo].type_ == PadType::DirectInput) {
			out = devJoysticks_[stickNo].state_.directInput_;
			return true;
		}
	}
	return false;
}

bool Input::GetJoystickStatePrevious(int32_t stickNo, DIJOYSTATE2& out)
{
	if (0 <= stickNo && stickNo < devJoysticks_.size()) {
		if (devJoysticks_[stickNo].type_ == PadType::DirectInput) {
			out = devJoysticks_[stickNo].statePre_.directInput_;
			return true;
		}
	}
	return false;
}

bool Input::GetJoystickState(int32_t stickNo, XINPUT_STATE& out)
{
    if (0 <= stickNo && stickNo < devJoysticks_.size()) {
        if (devJoysticks_[stickNo].type_ == PadType::XInput) {
            out = devJoysticks_[stickNo].state_.xInput_;
            return true;
        }
    }
    return false;
}

bool Input::GetJoystickStatePrevious(int32_t stickNo, XINPUT_STATE& out)
{
	if (0 <= stickNo && stickNo < devJoysticks_.size()) {
		if (devJoysticks_[stickNo].type_ == PadType::XInput) {
			out = devJoysticks_[stickNo].statePre_.xInput_;
			return true;
		}
	}
	return false;
}

void Input::SetJoystickDeadZone(int32_t stickNo, int32_t deadZoneL, int32_t deadZoneR) {
	if (0 <= stickNo && stickNo < devJoysticks_.size()) {
		devJoysticks_[stickNo].deadZoneL_ = (std::max)(0, (std::min)(deadZoneL, 0x8000));
		devJoysticks_[stickNo].deadZoneR_ = (std::max)(0, (std::min)(deadZoneR, 0x8000));
	}
}

void Input::SetupJoySticks()
{
	devJoysticks_.clear();
	// JoyStickの列挙
	HRESULT result = this->directInput_->EnumDevices(
		DI8DEVCLASS_GAMECTRL, EnumJoysticksCallback, this, DIEDFL_ATTACHEDONLY);
	assert(SUCCEEDED(result));

	// 入力データ・排他制御レベルのセット
	for (auto& joyStick : devJoysticks_) {
		result = joyStick.device_->SetDataFormat(&c_dfDIJoystick2);
		assert(SUCCEEDED(result));
		result =
			joyStick.device_->SetCooperativeLevel(hwnd_, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
		assert(SUCCEEDED(result));
		joyStick.deadZoneL_ = XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
		joyStick.deadZoneR_ = XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;
		// 軸のモードを絶対軸に設定
		DIPROPDWORD diprop;
		diprop.diph.dwSize = sizeof(DIPROPDWORD);
		diprop.diph.dwHeaderSize = sizeof(DIPROPHEADER);
		diprop.diph.dwHow = DIPH_DEVICE;
		diprop.diph.dwObj = 0;
		diprop.dwData = DIPROPAXISMODE_ABS;
		joyStick.device_->SetProperty(DIPROP_AXISMODE, &diprop.diph);

		sCurrentDevice = joyStick.device_.Get();
		joyStick.device_->EnumObjects(EnumAxesCallback, reinterpret_cast<void*>(hwnd_), DIDFT_AXIS);
	}

}

size_t Input::GetNumberOfJoysticks()
{
	return devJoysticks_.size();
}

bool Input::IsPressMouse(int32_t buttonNumber)
{
	return IsPress(mouse_, buttonNumber);
}

bool Input::IsTriggerMouse(int32_t buttonNumber)
{
	// 前回押してなくて、今回押していればトリガー
	bool currentPush = IsPressMouse(buttonNumber);
	return currentPush && !IsPress(preMouse_, buttonNumber);
}

Input::MouseMove Input::GetMouseMove()
{
	MouseMove tmp;
	tmp.lX = mouse_.lX;
	tmp.lY = mouse_.lY;
	tmp.lZ = mouse_.lZ;

	return tmp;
}

const Vector2& Input::GetMousePosition() const
{
	// TODO: return ステートメントをここに挿入します
	return mousePosition_;
}

BOOL Input::EnumJoysticksCallback(const DIDEVICEINSTANCE* pdidInstance, VOID* pContext) noexcept
{
	Input* input = static_cast<Input*>(pContext);
	Microsoft::WRL::ComPtr<IDirectInputDevice8> joystick;
	HRESULT result = input->directInput_->CreateDevice(pdidInstance->guidInstance, &joystick, nullptr);
	if (FAILED(result)) {
		return DIENUM_CONTINUE;
	}

	DIDEVICEINSTANCE instance;
	joystick->GetDeviceInfo(&instance);
	bool isXInput = IsXInputDevice(&pdidInstance->guidProduct);
	input->devJoysticks_.push_back(JoyStick{
		joystick,XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE,XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE,
		isXInput ? PadType::XInput : PadType::DirectInput });

	std::memset(&input->devJoysticks_.back().state_, 0, sizeof(State));

	return DIENUM_CONTINUE;
}

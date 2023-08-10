#include "Input.h"
#include "WinApp.h"
#include <cassert>

Input* Input::GetInstance()
{
    static Input instance;
    return &instance;
}

void Input::Initialize()
{
    WinApp* winApp = WinApp::GetInstance();
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

}

void Input::Update()
{
    // キーボード情報の取得開始
    dKeyBoard_->Acquire();
    dMouse_->Acquire();

    preKey_ = key_;

    // キーの入力
    key_.fill(0);
    dKeyBoard_->GetDeviceState(sizeof(key_), key_.data());

    // マウスの入力
    //std::memset(&)
    
}

bool Input::TrigerKey(BYTE keyNumber) const
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

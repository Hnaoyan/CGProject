#pragma once
#include <string>
#include <Windows.h>
#include <cstdint>
#include<format>

void Log(const std::string& message);

class WinApp 
{
public:
	// クライアント領域のサイズ
	static const int32_t kClientWidth = 1280;
	static const int32_t kClientHeight = 720;
	// ウインドウクラス名
	static const wchar_t kWindowClassName[];

public:	// メンバ関数
	/// <summary>
	/// ウィンドウプロシージャ
	/// </summary>
	/// <param name="hwnd"></param>
	/// <param name="msg"></param>
	/// <param name="wparam"></param>
	/// <param name="lparam"></param>
	/// <returns></returns>
	static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns></returns>
	static WinApp* GetInstance();

private:
	WinApp(){};
	~WinApp() {};

public:
	/// <summary>
	/// ゲームウィンドウの作成
	/// </summary>
	/// <param name="title">ウィンドウのタイトル</param>
	/// <param name="windowStyle"></param>
	/// <param name="clientWidth">ウィンドウのクライアント領域の横幅</param>
	/// <param name="clientHeight">ウィンドウのクライアント領	域の縦幅</param>
	void CreateGameWindow(const wchar_t* title = L"DirectXTitle", UINT windowStyle = WS_OVERLAPPEDWINDOW,
		int32_t clientWidth = kClientWidth, int32_t clientHeight = kClientHeight);

	/// <summary>
	/// メッセージの処理
	/// </summary>
	/// <returns>終了を確認</returns>
	bool ProcessMessage();

	HWND GetHwnd() const { return hwnd_; }

	HINSTANCE GetHInstance() const { return windowClass_.hInstance; }

private:// メンバ変数
	// Window関連
	HWND hwnd_ = nullptr;		// ウィンドウハンドル
	WNDCLASS windowClass_{};	// ウィンドウクラス

};

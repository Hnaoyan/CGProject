#include "WindowAPI.h"

#include "imgui.h"
#include "imgui_impl_dx12.h"
#include "imgui_impl_win32.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

//std::random_device WindowAPI::seedGenerator;
//std::mt19937 WindowAPI::randomEngine(WindowAPI::seedGenerator());

void Log(const std::string& message) {
	OutputDebugStringA(message.c_str());
}

LRESULT WindowAPI::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam)) {
		return true;
	}
	// メッセージに応じてゲーム固有の処理を行う
	switch (msg)
	{
	case WM_DESTROY:
		// OSに対してアプリの終了を伝える
		PostQuitMessage(0);
		return 0;
	}

	// 標準のメッセージ処理を行う
	return DefWindowProc(hwnd, msg, wparam, lparam);


}

const wchar_t WindowAPI::kWindowClassName[] = L"DirectXTitle";

void WindowAPI::CreateGameWindow(const wchar_t* title, UINT windowStyle, int32_t clientWidth, int32_t clientHeight) {
	// COM初期化
	CoInitializeEx(nullptr, COINIT_MULTITHREADED);

	// ウィンドウプロシージャ
	windowClass_.lpfnWndProc = WindowProc;
	// ウィンドウクラス名（なんでも良い）
	windowClass_.lpszClassName = kWindowClassName;
	// インスタンスハンドル
	windowClass_.hInstance = GetModuleHandle(nullptr);
	// カーソル
	windowClass_.hCursor = LoadCursor(nullptr, IDC_ARROW);

	// ウィンドウクラスを登録する
	RegisterClass(&windowClass_);

	// ウィンドウサイズ{ x, y, 横幅, 縦幅 }
	RECT wrc = { 0,0,clientWidth,clientHeight };
	// クライアント領域を基に実際のサイズにwrcを変更してもらう
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);

	// ウィンドウの生成
	hwnd_ = CreateWindow(
		windowClass_.lpszClassName,	// 利用するクラス名
		title,						// タイトルバーの文字
		windowStyle,		// よく見るウィンドウスタイル
		CW_USEDEFAULT,				// 表示X座標
		CW_USEDEFAULT,				// 表示Y座標
		wrc.right - wrc.left,		// ウィンドウ横幅
		wrc.bottom - wrc.top,		// ウィンドウ縦幅
		nullptr,					// 親ウィンドウハンドル
		nullptr,					// メニューハンドル
		windowClass_.hInstance,		// インスタンスハンドル
		nullptr);					// オプション

	// ウィンドウを表示する
	ShowWindow(hwnd_, SW_SHOW);
}

bool WindowAPI::ProcessMessage() {
	MSG msg{};
	// ウィンドウの×ボタンが押されるまでループ
	// Windowにメッセージが来てたら最優先で処理
	if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}
	if (msg.message == WM_QUIT) 
	{
		return true;
	}
	return false;
}

WindowAPI* WindowAPI::GetInstance() {
	static WindowAPI instance;
	return &instance;
}
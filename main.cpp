#include "DirectXCommon.h"
#include"WinApp.h"

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	WinApp* winApp;
	DirectXCommon* dxCommon;

	winApp = WinApp::GetInstance();

	dxCommon = DirectXCommon::GetInstance();
	dxCommon->Initialize(winApp);

	//winApp = WinApp::GetInstance();
	winApp->CreateGameWindow();

	while (winApp->ProcessMessage() == 0)
	{
		// 描画前処理
		dxCommon->PreDraw();
		// 描画後処理
		dxCommon->PostDraw();
	}

}

#include "MyGame.h"

void MyGame::Initialize()
{
	// 基底クラスの初期化
	Framework::Initialize();
	// シーンの初期化
	sceneManager = new SceneManager();
}

void MyGame::Finalize()
{
	// 基底クラスの終了処理
	Framework::Finalize();
}

void MyGame::Update()
{
	// 基底クラスの更新処理
	Framework::Update();

	endRequest_ = winApp->ProcessMessage();

	// 入力処理
	input->Update();
	// ImGui受付開始
	imguiManager->Begin();
	// ゲームシーン更新処理
	sceneManager->Update();
}

void MyGame::Draw()
{
	// 描画前処理
	dxCommon->PreDraw();

	// ゲームシーン描画処理
	sceneManager->Draw();

	// ImGui受付終了
	imguiManager->End();

	// ImGui描画
	imguiManager->Draw();

	// 描画後処理
	dxCommon->PostDraw();
}

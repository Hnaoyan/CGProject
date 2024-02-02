#include "MyGame.h"
#include "GlobalVariables.h"
#include "Editor/Editor.h"

void MyGame::Initialize()
{
	// 基底クラスの初期化
	Framework::Initialize();
	// グローバル変数の外部管理
	GlobalVariables::GetInstance()->LoadFiles();

	Editor::GetInstance()->LoadFiles();
	// シーンの初期化
	sceneFactory_ = new SceneFactory();
	SceneManager::GetInstance()->SetSceneFactory(sceneFactory_);
	SceneManager::GetInstance()->ChangeScene("TITLE");
}

void MyGame::Finalize()
{
	// シーンのDelete
	//SafeDelete(sceneManager);
	delete sceneFactory_;
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
	imguiManager_->Begin();
#ifdef _DEBUG
	// json更新
	GlobalVariables::GetInstance()->Update();
	// Editor
	//Editor::GetInstance()->Update();
#endif // _DEBUG

	// ゲームシーン更新処理
	SceneManager::GetInstance()->Update();
	//sceneManager->Update();
}

void MyGame::Draw()
{
	// 描画前処理
	dxCommon->PreDraw();

	// ゲームシーン描画処理
	SceneManager::GetInstance()->Draw();

	// ImGui受付終了
	imguiManager_->End();

	// ImGui描画
	imguiManager_->Draw();

	// 描画後処理
	dxCommon->PostDraw();
}

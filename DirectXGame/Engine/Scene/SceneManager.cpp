#include "SceneManager.h"
#include "imgui.h"
#include <assert.h>

SceneManager::SceneManager() 
{ 
	transitionManager_ = TransitionManager::GetInstance();
	transitionManager_->Initialize();
	isChangeNow_ = false;
}

SceneManager::~SceneManager() 
{
	delete nowScene_;
}

void SceneManager::Update() 
{ 
#ifdef _DEBUG

	ImGuiController();

#endif // _DEBUG

	GEYOU();

	// 遷移
	transitionManager_->Update();

	// 切り替え
	if (transitionManager_->ChangeTiming()) {
		assert(sceneFactory_);
		assert(nextScene_ == nullptr);

		nextScene_ = sceneFactory_->CreateScene(nextSceneName_);
		nextSceneName_ = "";
	}

	// 切り替え
	if (nextScene_) {
		if (nowScene_) {
			delete nowScene_;
		}
		// シーン切り替え
		nowScene_ = nextScene_;
		nextScene_ = nullptr;
		// 次のシーンの初期化
		nowScene_->Initialize();
	}
	// シーン更新処理
	nowScene_->Update();
}

void SceneManager::Draw() 
{ 
	nowScene_->Draw();
	transitionManager_->Draw();
}

void SceneManager::ImGuiController()
{

	if (transitionManager_->ChangeScene()) {
		isChangeNow_ = false;
	}

	ImGui::Begin("Controller");

	if (ImGui::Button("TITLE")) {
		if (!isChangeNow_) {
			ChangeScene("TITLE");
		}
	}
	if (ImGui::Button("GAME")) {
		if (!isChangeNow_) {
			ChangeScene("GAME");
		}
	}
	if (ImGui::Button("CLEAR")) {
		if (!isChangeNow_) {
			ChangeScene("CLEAR");
		}
	}
	if (ImGui::Button("SAMPLE")) {
		if (!isChangeNow_) {
			ChangeScene("SAMPLE");
		}
	}

	ImGui::End();

}

void SceneManager::ChangeScene(const std::string & sceneName)
{
	assert(sceneFactory_);
	assert(nextScene_ == nullptr);

	isChangeNow_ = true;

	if (nowScene_ == nullptr) {
		nextScene_ = sceneFactory_->CreateScene(sceneName);
		return;
	}

	// 次のシーン生成
	nextSceneName_ = sceneName;
	transitionManager_->Start();
	//nextScene_ = sceneFactory_->CreateScene(sceneName);

}

void SceneManager::GEYOU()
{
	if (transitionManager_->ChangeScene()) {
		isChangeNow_ = false;
	}
	if (Input::GetInstance()->TriggerKey(DIK_LSHIFT)) {
		if (!isChangeNow_) {
			ChangeScene("SAMPLE");
		}
	}
	if (Input::GetInstance()->TriggerKey(DIK_RSHIFT)) {
		if (!isChangeNow_) {
			ChangeScene("TITLE");
		}
	}

}

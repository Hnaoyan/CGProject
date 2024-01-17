#include "TitleScene.h"
#include "TextureManager.h"
#include "imgui.h"
#include "SceneManager.h"

void TitleScene::Initialize()
{
	input_ = Input::GetInstance();
	dxCommon_ = DirectXCommon::GetInstance();

	buttonInfo_.position = { 1280 / 2, 460 };
	buttonInfo_.anchor = { 0.5f,0.5f };
	buttonInfo_.color = { 1,1,1,1 };

	uint32_t buttonTex = TextureManager::Load("UI/Button.png");

	buttonSprite_.reset(Sprite::Create(buttonTex,
		buttonInfo_.position, buttonInfo_.color,
		buttonInfo_.anchor, false, false));
	uint32_t texture = TextureManager::Load("UI/BackGround.png");
	back_.reset(Sprite::Create(texture, { 0,0 }, { 1,1,1,1 }, { 0,0 }, false, false));

	texture = TextureManager::Load("UI/Title.png");
	textInfo_.position = { 1280 / 2, 380 };
	textInfo_.anchor = { 0.5f,0.5f };
	textInfo_.color = { 1,1,1,1 };
	textSprite_.reset(Sprite::Create(texture, textInfo_.position, textInfo_.color,
		textInfo_.anchor, false, false));

	Vector2 size = textSprite_->GetSize();
	textSprite_->SetSize(size * 2);
	size = buttonSprite_->GetSize();
	buttonSprite_->SetSize(size * 2);

}

void TitleScene::Update()
{
#ifdef _DEBUG
	if (input_->TriggerKey(DIK_RSHIFT)) {
		sceneNum++;
	}
	if (input_->TriggerKey(DIK_LSHIFT)) {
		sceneNum = 2;
	}

	ImGui::Begin("sprite");
	ImGui::DragFloat2("bPos", &buttonInfo_.position.x, 0.1f, 0, 1280);
	ImGui::DragFloat2("tPos", &textInfo_.position.x, 0.1f, 0, 1280);
	ImGui::End();

	buttonSprite_->SetPosition(buttonInfo_.position);
	textSprite_->SetPosition(textInfo_.position);

#endif // _DEBUG

	XINPUT_STATE joyState;
	if (input_->GetJoystickState(0, joyState)) {
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A) {
			SceneManager::GetInstance()->ChangeScene("GAME");
		}
	}

	if (input_->TriggerKey(DIK_SPACE)) {
		SceneManager::GetInstance()->ChangeScene("GAME");
	}

}

void TitleScene::Draw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 描画前処理
	Sprite::PreDraw(commandList);

	buttonSprite_->Draw();
	textSprite_->Draw();
	back_->Draw();

	// 描画後処理
	Sprite::PostDraw();
#pragma endregion
}

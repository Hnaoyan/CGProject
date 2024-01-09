#include "ClearScene.h"

void ClearScene::Initialize()
{
	input_ = Input::GetInstance();
	dxCommon_ = DirectXCommon::GetInstance();

	buttonInfo_.position = { 700, 460 };
	buttonInfo_.anchor = { 0.5f,0.5f };
	buttonInfo_.color = { 1,1,1,1 };

	uint32_t buttonTex = TextureManager::Load("UI/ClearB.png");

	buttonSprite_.reset(Sprite::Create(buttonTex,
		buttonInfo_.position, buttonInfo_.color,
		buttonInfo_.anchor, false, false));
	uint32_t texture = TextureManager::Load("UI/BackGround.png");
	back_.reset(Sprite::Create(texture, { 0,0 }, { 1,1,1,1 }, { 0,0 }, false, false));

	texture = TextureManager::Load("UI/ClearText.png");
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

void ClearScene::Update()
{
#ifdef _DEBUG
	if (input_->TriggerKey(DIK_RSHIFT)) {
		sceneNum++;
	}
	if (input_->TriggerKey(DIK_LSHIFT)) {
		sceneNum = 2;
	}
#endif // _DEBUG

	XINPUT_STATE joyState;
	if (input_->GetJoystickState(0, joyState)) {
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_B) {
			sceneNum = 0;
		}
	}
	if (input_->TriggerKey(DIK_SPACE)) {
		sceneNum = 0;
	}

}

void ClearScene::Draw()
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

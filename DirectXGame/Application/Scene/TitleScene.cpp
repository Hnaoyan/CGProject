#include "TitleScene.h"

void TitleScene::Initialize()
{
	input_ = Input::GetInstance();
}

void TitleScene::Update()
{
#ifdef _DEBUG
	if (input_->TriggerKey(DIK_RSHIFT)) {
		sceneNum++;
	}
#endif // _DEBUG

	XINPUT_STATE joyState;
	if (input_->GetJoystickState(0, joyState)) {
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A) {
			sceneNum++;
		}
	}

}

void TitleScene::Draw()
{

}

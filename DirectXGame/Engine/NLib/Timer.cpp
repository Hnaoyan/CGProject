#include "Timer.h"

void Timer::StartSetting()
{
	// リセット
	isEnd_ = false;
	nowFrame_ = 0;
	// 開始
	isActive_ = true;
}

void Timer::Start(float endFrame)
{
	// 初期化
	StartSetting();

	// 終了フレーム
	endFrame_ = endFrame;

}

void Timer::End()
{
	isEnd_ = true;
	isActive_ = false;
}

void Timer::Update()
{

	if (isActive_) {

		if (nowFrame_ >= 1.0f) {
			//isEnd_ = true;
			//isActive_ = false;
			End();
			return;
		}

		this->nowFrame_ += (1.0f / endFrame_);

	}
}

#include "TimedCall.h"

TimedCall::TimedCall(std::function<void()> function, uint32_t time) : function_(function), time_(time) {}

void TimedCall::Update() {

	if (isFinished) {
		return;
	}
	time_--;
	if (time_ <= 0) {
		isFinished = true;
		//コールバック関数の呼び出し
		function_();
	}

}

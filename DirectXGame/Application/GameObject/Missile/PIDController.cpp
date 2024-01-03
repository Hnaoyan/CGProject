#include "PIDController.h"

float PIDController::Calculate(float setPoint, float currentValue)
{
	float error = setPoint - currentValue;

	// 比例項
	float pTerm = kp * error;

	// 積分項
	integral += error;
	float iTerm = kp * integral;

	// 微分項
	float dTerm = kd * (error - prevError);
	prevError = error;

	// PID制御出力
	float output = pTerm + iTerm + dTerm;

	return output;
}

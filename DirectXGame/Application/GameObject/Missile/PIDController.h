#pragma once
class PIDController
{
private:

	float kp, ki, kd, prevError, integral;

public:

	PIDController(float p, float i, float d) : kp(p), ki(i), kd(d), prevError(0), integral(0) {}

	float Calculate(float setPoint, float currentValue);

};


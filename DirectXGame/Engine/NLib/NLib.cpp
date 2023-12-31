#include "NLib.h"
#include "MathCalc.h"
#include <Windows.h>

float NLib::InductionPerform(float frame, float accuracy)
{
    return (frame - 1.0f) + accuracy;
}

Vector3 NLib::InductionNewVector(const Vector3& missile, const Vector3& target, float p)
{
    Vector3 result = {};

    Vector3 t1 = VectorLib::Scaler(missile, (1.0f - p));
    Vector3 t2 = VectorLib::Scaler(target, p);

    result = (t1 / MathCalc::Normalize(t2)) * MathCalc::Normalize(missile);

    return result;
}

Vector3 NLib::Slerp(const Vector3& moveVector, const Vector3& targetVector, float t)
{
    Vector3 result = {};
    Vector3 mvNormalize = MathCalc::Normalize(moveVector);
    Vector3 trNormalize = MathCalc::Normalize(targetVector);

    float dot = MathCalc::Dot(mvNormalize, trNormalize);

    if (dot < -0.999f) {
        result = VectorLib::Scaler(mvNormalize, 1.0f - t);
        result += VectorLib::Scaler(trNormalize, 1.0f - t);
        return result;
    }

    // 角度
    float theta = std::acosf(dot);
    float sinTheta = std::sinf(theta);

    float s0 = std::sinf((1.0f - t) * theta) / sinTheta;
    float s1 = std::sinf(t * theta) / sinTheta;

    result = VectorLib::Scaler(mvNormalize, s0) + VectorLib::Scaler(trNormalize, s1);

    return MathCalc::Normalize(result);
}

Vector3 NLib::Slerp_Test(const Vector3& moveVector, const Vector3& targetVector, float t)
{
    //v1,v2を正規化
    Vector3 start = MathCalc::Normalize(moveVector);
    Vector3 end = MathCalc::Normalize(targetVector);

    //2ベクトル間の角度(鋭角側)
    float angle = std::acosf(MathCalc::Dot(start, end));

    //補間係数
    float Ps = std::sinf(angle * (1 - t));
    float Pe = std::sinf(angle * t);

    Vector3 result = VectorLib::Multiply(1.0f / std::sinf(angle), VectorLib::Add(VectorLib::Multiply(Ps, start), VectorLib::Multiply(Pe, end)));

    return result;
}

float NLib::DeltaTimeSpeed(float kSpeed)
{
    float deltaTime = 0;
    DEVMODE dm;
    ZeroMemory(&dm, sizeof(dm));
    dm.dmSize = sizeof(dm);

    if (EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &dm) != 0) {
        deltaTime = static_cast<float>(dm.dmDisplayFrequency);
    }

    return kSpeed * (1.0f / deltaTime);
}

float NLib::GetDeltaTime(float frame)
{
    return 1.0f / frame;
}

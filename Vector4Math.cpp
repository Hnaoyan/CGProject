#include "Vector4Math.h"

Vector4 Vector4Math::Add(const Vector4& v1, const Vector4& v2)
{
    Vector4 result{};

    result.x = v1.x + v2.x;
    result.y = v1.y + v2.y;
    result.z = v1.z + v2.z;
    result.w = v1.w + v2.w;

    return result;
}

#include "Framework.h"

GameMath::GameMath()
    : generator(randomDevice()), floatdistribution(0.0f, 1.0f),
    intdistribution(0)
{
}

int GameMath::Random(const int& min, const int& max)
{
    return intdistribution(generator) % (max - min) + min;
}

float GameMath::Random(const float& min, const float& max)
{
    return (max - min) * floatdistribution(generator) + min;
}

float GameMath::Clamp(const float& min, const float& max, const float& value) const
{
    if (value < min)
        return min;

    if (value > max)
        return max;

    return value;
}


Vector3 GameMath::Lerp(const Vector3& start, const Vector3 end, float& t) const
{
    t = Clamp(0.0f, 1.0f, t);

    return start + (end - start) * t;
}

Matrix GameMath::Lerp(const Matrix& start, const Matrix end, float& t) const
{
    t = Clamp(0.0f, 1.0f, t);

    return start + (end - start) * t;
}

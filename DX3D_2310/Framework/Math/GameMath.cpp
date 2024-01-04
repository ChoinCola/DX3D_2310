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

Vector3 GameMath::Random(const Vector3& min, const Vector3& max)
{
    return Vector3(Random(min.x, max.x), Random(min.y, max.y), Random(min.z, max.z));
}

float GameMath::Clamp(const float& min, const float& max, const float& value) const
{
    if (value < min)
        return min;

    if (value > max)
        return max;

    return value;
}

float GameMath::Lerp(const float& start, const float& end, float t) const
{
    t = Clamp(0.0f, 1.0f, t);

    return start + (end - start) * t;
}

Vector3 GameMath::Lerp(const Vector3& start, const Vector3& end, float t) const
{
    t = Clamp(0.0f, 1.0f, t);

    return start + (end - start) * t;
}

Matrix GameMath::Lerp(const Matrix& start, const Matrix& end, float t) const
{
    t = Clamp(0.0f, 1.0f, t);

    return start + (end - start) * t;
}

Vector3 GameMath::ClosestPointOnLine(const Vector3& start, const Vector3& end, const Vector3& point) const
{
    Vector3 line = end - start;
    Vector3 A = point - start;


    float x = Vector3::Dot(line, A);
    float y = Vector3::Dot(line, line);

    float t = Clamp(0.0f, 1.0f, x / y);
    return start + line * t;
}

float GameMath::Distance(const Vector3& v1, const Vector3& v2)
{
    return (v1 - v2).Length();
}

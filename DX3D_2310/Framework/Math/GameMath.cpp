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

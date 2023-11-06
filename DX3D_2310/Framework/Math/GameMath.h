#pragma once

class GameMath : public Singleton<GameMath>
{
public:
	bool NearlyEqual(const float& s1, const float& s2)
	{
		return abs(s1 - s2) < FLT_EPSILON; // float에서 가장 작은값.
	}
};
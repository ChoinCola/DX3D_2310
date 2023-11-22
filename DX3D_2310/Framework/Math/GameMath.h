#pragma once

class GameMath : public Singleton<GameMath>
{
private:
	friend class Singleton;

	GameMath();
	~GameMath() = default;

public:

	bool NearlyEqual(const float& s1, const float& s2)
	{
		return abs(s1 - s2) < FLT_EPSILON; // float에서 가장 작은값.
	}

	int Random(const int& min, const int& max);
	float Random(const float& min, const float& max);

	float Clamp(const float& min, const float& max, const float& value) const;

	Vector3 Lerp(const Vector3& start, const Vector3& end, float t) const;
	Matrix Lerp(const Matrix& start, const Matrix& end, float t) const;

private:
	random_device randomDevice;
	mt19937_64 generator;

	uniform_int_distribution<int> intdistribution;
	uniform_real_distribution<float> floatdistribution;
};
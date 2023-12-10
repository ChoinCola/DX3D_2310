#pragma once
class FireBall : public SphereCollider
{
private:
	const float LIFE_TIME = 2.0f;

public:
	FireBall(Transform* transform);
	~FireBall();

	void Update();

	void Fire(Vector3 pos, Vector3 direction);
private:
	Vector3 velocity;
	float speed = 20.0f;
	float lifeTime = 0.0f;
	Transform* meshtransform;
};
#include "Framework.h"

FireBall::FireBall(Transform* transform) : meshtransform(transform)
{
	SetTag("FireBallCollider");
	Load();

	transform->SetParent(this);
	transform->Load();

	isActive = false;
}

FireBall::~FireBall()
{
	delete meshtransform;
}

void FireBall::Update()
{
	if (!IsActive()) return;

	lifeTime += DELTA;

	Translate(velocity * speed * DELTA);

	if (lifeTime > LIFE_TIME)
		isActive = false;
	UpdateWorld();
}

void FireBall::Fire(Vector3 pos, Vector3 direction)
{
	SetLocalPosition(pos);
	velocity = direction.GetNormalized();

	isActive = true;
	lifeTime = 0.0f;
}

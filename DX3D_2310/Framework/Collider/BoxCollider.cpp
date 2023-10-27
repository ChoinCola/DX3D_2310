#include "Framework.h"

bool BoxCollider::IsRayCollision(IN const Ray& ray, OUT Contact* contact)
{
    return false;
}

bool BoxCollider::IsBoxCollision(BoxCollider* collider)
{
    return false;
}

bool BoxCollider::IsSphereCollision(SphereCollider* collider)
{
    return false;
}

bool BoxCollider::IsCapsuleCollision(CapsuleCollider* collider)
{
    return false;
}

void BoxCollider::MakeMesh()
{
}

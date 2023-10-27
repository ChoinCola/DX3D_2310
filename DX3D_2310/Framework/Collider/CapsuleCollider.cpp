#include "Framework.h"

bool CapsuleCollider::IsRayCollision(IN const Ray& ray, OUT Contact* contact)
{
    return false;
}

bool CapsuleCollider::IsBoxCollision(BoxCollider* collider)
{
    return false;
}

bool CapsuleCollider::IsSphereCollision(SphereCollider* collider)
{
    return false;
}

bool CapsuleCollider::IsCapsuleCollision(CapsuleCollider* collider)
{
    return false;
}

void CapsuleCollider::MakeMesh()
{
}

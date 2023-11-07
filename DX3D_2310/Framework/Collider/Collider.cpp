#include "Framework.h"

Collider::Collider() : GameObject(L"Basic/Collider.hlsl")
{
    mesh = new Mesh<Vertex>();
    SetColor(0, 1, 0);
}

Collider::~Collider()
{
    delete mesh;
}

void Collider::Render()
{

        SetRender();
        mesh->Draw(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);


}

bool Collider::IsCollision(Collider* collider)
{
    switch (collider->type)
    {
    case Collider::BOX:
        return IsBoxCollision((BoxCollider*)collider);
        break;
    case Collider::SPHERE:
        return IsSphereCollision((SphereCollider*)collider);
        break;
    case Collider::CAPSULE:
        return IsCapsuleCollision((CapsuleCollider*)collider);
        break;
    }

    return false;
}

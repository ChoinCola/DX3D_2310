#include "Framework.h"

CollisionScene::CollisionScene()
{
	colliders.push_back(new SphereCollider());
	colliders.back()->SetTag("Sphere1");
	//colliders.push_back(new SphereCollider());
	//colliders.back()->SetTag("Sphere2");
    colliders.push_back(new BoxCollider());
    colliders.back()->SetTag("Box1");
    //colliders.push_back(new BoxCollider());
    //colliders.back()->SetTag("Box2");
}

CollisionScene::~CollisionScene()
{
	for(auto def : colliders)
		delete def;
}

void CollisionScene::Update()
{
    //Ray ray = CAM->ScreenPointToRay(Mouse::Get()->GetPosition());

    //Contact contact;

    //if (colliders[0]->IsRayCollision(ray, &contact))
    //{
    //    colliders[0]->SetColor(1, 0, 0); 
    //}
    //else
    //{
    //    colliders[0]->SetColor(0, 1, 0);
    //}

    if (colliders[0]->IsCollision(colliders[1]))
    {
        colliders[0]->SetColor(1, 0, 0);
        colliders[1]->SetColor(1, 0, 0);
    }
    else
    {
        colliders[0]->SetColor(0, 1, 0);
        colliders[1]->SetColor(0, 1, 0);
    }


    for (Collider* collider : colliders)
        collider->UpdateWorld();
}

void CollisionScene::PreRender()
{
}

void CollisionScene::Render()
{
    for (Collider* collider : colliders)
        collider->Render();
}

void CollisionScene::PostRender()
{
}

void CollisionScene::GUIRender()
{
    for (Collider* collider : colliders)
        collider->GUIRender();
}

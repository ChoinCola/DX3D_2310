#include "Framework.h"

ParticleScene::ParticleScene()
{
    collider = new SphereCollider(50);
    collider->UpdateWorld();

    //particle = new ParticleSystem("TextData/FX/Snow.fx");
    //particle = new Sprite(L"Textures/FX/explosion.png", 30, 30, 5, 3);
    particle = new Snow();
}

ParticleScene::~ParticleScene()
{
    delete collider;
    delete particle;
}

void ParticleScene::Update()
{
    if (KEY->Down(VK_LBUTTON))
    {
        Ray ray = CAM->ScreenPointToRay(mousePos);
        Contact contact;

        if (collider->IsRayCollision(ray, &contact))
        {
            particle->Play(contact.hitPoint);
        }
    }

    particle->Update();
}

void ParticleScene::PreRender()
{
}

void ParticleScene::Render()
{
    collider->Render();
    particle->Render();
}

void ParticleScene::PostRender()
{
}

void ParticleScene::GUIRender()
{
    particle->GUIRender();
}
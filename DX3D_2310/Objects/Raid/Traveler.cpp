#include "Framework.h"
#include "Traveler.h"

Traveler::Traveler()
{
    bodyMesh = new ModelAnimator("Traveler");
    bodyMesh->SetShader(L"Model/2DTextureModel.hlsl");
    bodyMesh->Load();
    bodyMesh->SetParent(this);
    ReadClips();

    rightHand = new Transform();

    sword = new Model("weapon0");
    sword->SetParent(rightHand);
    sword->Load();
}

Traveler::~Traveler()
{
    delete bodyMesh;
    delete sword;
    delete rightHand;
}

void Traveler::Update()
{
    Attack();

    rightHand->SetWorld(bodyMesh->GetTransformByNode(36));

    UpdateWorld();
    bodyMesh->Update();
    sword->UpdateWorld();
}

void Traveler::Render()
{
    bodyMesh->Render();
    sword->Render();
}

void Traveler::GUIRender()
{
    bodyMesh->GUIRender();
    sword->GUIRender();
}

void Traveler::Attack()
{
    if (KEY->Down(VK_LBUTTON))
    {
        SetState(ATTACK);
    }
}

void Traveler::EndAttack()
{
    SetState(IDLE);
}

void Traveler::SetState(ActionState state)
{
    if (curState == state) return;

    curState = state;
    bodyMesh->PlayClip(state);
}

void Traveler::ReadClips()
{
    bodyMesh->ReadClip("Idle");
    bodyMesh->ReadClip("Walk");
    bodyMesh->ReadClip("Flair");
    bodyMesh->CreateTexture();

    bodyMesh->GetClip(ATTACK)->SetEvent(bind(&Traveler::EndAttack, this), 0.8f);
}

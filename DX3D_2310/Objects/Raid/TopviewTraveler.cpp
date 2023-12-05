#include "Framework.h"

TopviewTraveler::TopviewTraveler()
{
    SetTag("TopviewTraveler_collider");
    Load();

    bodyMesh = new ModelAnimator("Traveler");
    bodyMesh->SetShader(L"Model/2DTextureModel.hlsl");
    bodyMesh->Load();
    bodyMesh->SetParent(this);
    bodyMesh->SetLocalRotation(Vector3(0, 135, 0));
    ReadClips();
}

TopviewTraveler::~TopviewTraveler()
{
	delete bodyMesh;
}

void TopviewTraveler::Update()
{
    SearchPlayer();
    Control();
    Move();

    SetAction();
    UpdateWorld();
    bodyMesh->Update();
}

void TopviewTraveler::Render()
{
    bodyMesh->Render();
    __super::Render();
}

void TopviewTraveler::GUIRender()
{
    bodyMesh->GUIRender();
    __super::GUIRender();
}

void TopviewTraveler::Attack()
{
    FireBallManager::Get()->Fire(localPosition, GetForward());
}

void TopviewTraveler::EndAttack()
{
    SetState(IDLE);
}

void TopviewTraveler::Control()
{
    if (curState == ATTACK) return;
    if (KEY->Press('W'))
        velocity.z = 1.0f;
    else if (KEY->Press('S'))
        velocity.z = -1.0f;
    else
        velocity.z = 0.0f;

    if (KEY->Press('A'))
        velocity.x = -1.0f;
    else if (KEY->Press('D'))
        velocity.x = +1.0f;
    else
        velocity.x = 0.0f;

    if (KEY->Down(VK_LBUTTON))
    {
        SetState(ATTACK);
    }

}

void TopviewTraveler::Move()
{
    if (curState == ATTACK) return;

    if (velocity.Length() < EPSILON)
        return;

    Translate(velocity * moveSpeed * DELTA);

    //localRotation.y = atan2(velocity.x, velocity.z);
    Vector3 cross = Vector3::Cross(GetForward(), velocity);

    if (cross.y > EPSILON)
        Rotate(Vector3::Up() * rotSpeed * DELTA);
    else if (cross.y < -EPSILON)
        Rotate(Vector3::Down() * rotSpeed * DELTA);
    else if (GetForward().GetNormalized() != velocity.GetNormalized())
    {
        Rotate(Vector3::Up() * rotSpeed * DELTA);
    }

}

void TopviewTraveler::SetAction()
{
    if (curState == ATTACK) return;

    if (velocity.Length() > EPSILON)
        SetState(WALK);
    else
        SetState(IDLE);
}

void TopviewTraveler::SetState(ActionState state)
{
    if (curState == state) return;

    curState = state;
    bodyMesh->PlayClip(state);
}

void TopviewTraveler::ReadClips()
{
    bodyMesh->ReadClip("Idle");
    bodyMesh->ReadClip("Walk");
    bodyMesh->ReadClip("Attack");
    bodyMesh->CreateTexture();

    bodyMesh->GetClip(ATTACK)->SetEvent(bind(&TopviewTraveler::Attack, this), 0.1f);
    bodyMesh->GetClip(ATTACK)->SetEvent(bind(&TopviewTraveler::EndAttack, this), 0.8f);
}

void TopviewTraveler::SearchPlayer()
{
}

void TopviewTraveler::Hit(float damage)
{
    curHP -= damage;
}

void TopviewTraveler::Death()
{
}

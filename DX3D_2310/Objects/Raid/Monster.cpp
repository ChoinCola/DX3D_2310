#include "Framework.h"
#include "Monster.h"

Monster::Monster()
{
    SetTag("Monster_collider");
    Load();

    bodyMesh = new ModelAnimator("Monster");
    bodyMesh->Load();
    bodyMesh->SetParent(this);

    Searchcollision = new SphereCollider();
    Searchcollision->SetTag("MonsterSearchCollision");
    Searchcollision->SetParent(this);
    Searchcollision->SetColor(0, 0, 1);
    Searchcollision->Load();

    ReadClips();
}

Monster::~Monster()
{
    delete Searchcollision;
    delete bodyMesh;
}

void Monster::Update()
{
    SearchPlayer();
    Attack();
    Control();
    //Move();

    SetAction();

    UpdateWorld();
    bodyMesh->Update();
    Searchcollision->UpdateWorld();
}

void Monster::Render()
{
    bodyMesh->Render();
    Searchcollision->Render();
    __super::Render();
}

void Monster::GUIRender()
{
    ImGui::DragInt("Swordtransform", &sword_transform, 1.0f, 0, 199);
    bodyMesh->GUIRender();
    Searchcollision->GUIRender();

    ImGui::Text(to_string(SearchTimeNow).c_str());

    string def = PlayerLastPos;
    ImGui::Text(def.c_str());
    def = PtoMdir;
    ImGui::Text(def.c_str());
    __super::GUIRender();
}

void Monster::Attack()
{
    if (Playercol != nullptr && IsCollision(Playercol))
    {
        SetState(ATTACK);
    }
}

void Monster::EndAttack()
{
    SetState(IDLE);
}

void Monster::Control()
{
    if (curState == ATTACK) return;

    // Rot
    Vector3 nowR, plyR;
    nowR = GetGlobalPosition() + GetForward();

    plyR = PlayerLastPos + GetGlobalPosition();


    nowR.y = 0;
    nowR.Normalized();
    plyR.y = 0;
    plyR.Normalized();
    Vector3 rotate = Vector3::Cross(nowR, plyR);
    rotate.Normalized();
    PtoMdir = rotate;

    // Move
    if (PlayerLastPos != Vector3::Zero())
    {
        veloctiy.z += accelation * DELTA;
    }

    if (veloctiy.Length() > 1.0f)
        veloctiy.Normalized();


    if (rotate.y == 0) return;
    Rotate(rotate * rotSpeed * DELTA);
}

void Monster::Move()
{
    if (curState == ATTACK) return;
    if (PlayerLastPos == Vector3::Zero()) return;

    Matrix rotY = XMMatrixRotationY(localRotation.y);
    Vector3 direction = XMVector3TransformNormal(veloctiy, rotY);

    Translate(direction * moveSpeed * DELTA);
}

void Monster::SetAction()
{
    if (curState == ATTACK) return;

    if (veloctiy.z > EPSILON)
        SetState(WALK);
    else
        SetState(IDLE);
}

void Monster::SetState(ActionState state)
{
    if (curState == state) return;

    curState = state;
    bodyMesh->PlayClip(state);

}

void Monster::ReadClips()
{
    bodyMesh->ReadClip("Idle");

    bodyMesh->ReadClip("Walk");

    bodyMesh->ReadClip("Hit1");
    bodyMesh->ReadClip("Hit2");
    bodyMesh->ReadClip("Hit3");
    bodyMesh->ReadClip("Hit4");

    bodyMesh->ReadClip("Attack1");
    bodyMesh->ReadClip("Attack2");
    bodyMesh->CreateTexture();

    bodyMesh->GetClip(ATTACK)->SetEvent(bind(&Monster::EndAttack, this), 0.8f);
    bodyMesh->GetClip(ATTACK2)->SetEvent(bind(&Monster::EndAttack, this), 0.8f);
}

void Monster::SearchPlayer()
{
    if (Playercol == nullptr) return;

    if (Searchcollision->IsCollision(Playercol))
    {
        PlayerLastPos = Playercol->GetGlobalPosition();
        SearchTimeNow = .0f;
        AttackPos = true;
    }
    
    if (PlayerLastPos == Vector3::Zero()) return;

    if (SearchTimeNow < SearchTime)
    {
        SearchTimeNow += DELTA;
    }
    else if (SearchTimeNow >= SearchTime)
    {
        PlayerLastPos = Vector3::Zero();
        SearchTimeNow = .0f;
        AttackPos = false;
    }
    
    Vector3 nowpos = GetGlobalPosition();
    nowpos.y = 0;

    Vector3 targetpos = PlayerLastPos;
    targetpos.y = 0;

    if (PlayerLastPos != Vector3::Zero() && nowpos == targetpos)
    {
        PlayerLastPos = Vector3(0, 0, 0);
    }
}

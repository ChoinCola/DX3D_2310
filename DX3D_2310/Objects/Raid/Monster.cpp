#include "Framework.h"
#include "Monster.h"

Monster::Monster()
{
    SetTag("Monster_collider");
    Load();


    bodyMesh = new ModelAnimator("Monster");
    bodyMesh->Load();
    bodyMesh->SetParent(this);

    hpbar = new HPbar("Monster", this);
    Searchcollision = new SphereCollider();

    Searchcollision->SetTag("MonsterSearchCollision");
    Searchcollision->SetParent(this);
    Searchcollision->SetColor(0, 0, 1);
    Searchcollision->Load();

    ReadClips();

    Observer::Get()->AddParamEvent("MonsterHit", bind(&Monster::Hitnow, this, placeholders::_1));
}

Monster::~Monster()
{
    delete Searchcollision;
    delete bodyMesh;
}

void Monster::Update()
{
    hpbar->UpdateHPbar(HP, 100.0f);
    if (HP <= 0) SetState(DEATH);
    if (death == false) {
        bodyMesh->Update();
        if (curState == DEATH) return;
        SearchPlayer();
        Attack();
        Control();
        Move();

        SetAction();
        UpdateWorld();
        Searchcollision->UpdateWorld();
    }
}

void Monster::Render()
{
    bodyMesh->Render();
    Searchcollision->Render();
    __super::Render();
}

void Monster::GUIRender()
{
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
    if (curState >= HIT && curState <= HIT4) return;
    if (Playercol != nullptr && IsCollision(Playercol))
    {
        SetState(ATTACK);
    }
}

void Monster::EndAttack()
{
    SetState(IDLE);
    Hitself = false;
}

void Monster::Hitnow(void* collider)
{
    if (Hitself == false) {
        Sword* nowcol = static_cast<Sword*>(collider);
        if (IsCollision(nowcol->GetCollider())) {
            SetState((ActionState)MATH->Get()->Random(HIT, HIT4 + 1));
            Hitself = true;

            HP -= nowcol->GetDamage();
        }
    }
}

void Monster::Control()
{
    if (curState == ATTACK) return;
    if (PlayerLastPos == Vector3::Zero()) {
        veloctiy = Vector3::Zero();
        return;
    }

    // Rot
    Vector3 PtoMro = PlayerLastPos - GetGlobalPosition();
    Vector3 MonsterForward = GetForward();

    float Rdot = Vector3::Dot(PtoMro, MonsterForward);

    float rad = (float)acos(Rdot);

    // 유저가 좌측인이 우측인지.
    float rotate = Vector3::Dot(GetRight(), PtoMro);
    rotate < 0 ? rotate = 1 : rotate = -1;
    PtoMdir.y = rotate;

    // Move
    if (PlayerLastPos != Vector3::Zero())
    {
        veloctiy.z += accelation * DELTA;
    }

    if (veloctiy.Length() > 1.0f)
        veloctiy.Normalized();


    if (rotate == 0) return;
    Rotate(Vector3(0, rotate * rotSpeed * DELTA, 0));
}

void Monster::Move()
{
    if (curState == ATTACK) return;
    if (PlayerLastPos == Vector3::Zero()) return;
    if (curState >= HIT && curState <= HIT4) return;

    Matrix rotY = XMMatrixRotationY(localRotation.y);
    Vector3 direction = XMVector3TransformNormal(veloctiy, rotY);

    Translate(direction * -moveSpeed * DELTA);
}

void Monster::SetAction()
{
    if (curState == ATTACK) return;
    if (curState >= HIT && curState <= HIT4) return;

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

    bodyMesh->ReadClip("death");

    bodyMesh->CreateTexture();

    bodyMesh->GetClip(ATTACK)->SetEvent(bind(&Monster::EndAttack, this), 0.8f);
    bodyMesh->GetClip(ATTACK2)->SetEvent(bind(&Monster::EndAttack, this), 0.8f);

    bodyMesh->GetClip(HIT)->SetEvent(bind(&Monster::EndAttack, this), 0.8f);
    bodyMesh->GetClip(HIT2)->SetEvent(bind(&Monster::EndAttack, this), 0.8f);
    bodyMesh->GetClip(HIT3)->SetEvent(bind(&Monster::EndAttack, this), 0.8f);
    bodyMesh->GetClip(HIT4)->SetEvent(bind(&Monster::EndAttack, this), 0.8f);
    bodyMesh->GetClip(DEATH)->SetEvent(bind(&Monster::Death, this), 0.9f);
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

void Monster::Death()
{
    death = true;
}

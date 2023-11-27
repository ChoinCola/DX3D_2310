#include "Framework.h"
#include "Traveler.h"

Traveler::Traveler()
{
    UIRenderMaster::Get()->InputUI2D("Travler_HP", L"Textures/Colors/Red.png");
    HPbar = UIRenderMaster::Get()->GetUI2D("Travler_HP");
    SetTag("Travler_collider");
    Load();

    bodyMesh = new ModelAnimator("Traveler");
    bodyMesh->SetShader(L"Model/2DTextureModel.hlsl");
    bodyMesh->Load();
    bodyMesh->SetParent(this);
    bodyMesh->SetLocalRotation(Vector3(0, 135, 0));
    ReadClips();

    rightHand = new Transform();

    sword = new Sword();
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
    UIRenderMaster::Get()->Set2DUIScaleAtSort
    ("Travler_HP",1 ,HP * 0.01, 1.0f, Vector3(150, 100, 1), Vector3(1, 0.1, 1));
    if (curState == ATTACK) Observer::Get()->ExcuteParamEvent("MonsterHit", sword);
    attack = nullptr;
    Attack();
    Control();
    Move();
    SetAction();

    rightHand->SetWorld(bodyMesh->GetTransformByNode(sword_transform));

    UpdateWorld();
    bodyMesh->Update();
    sword->Update();
}

void Traveler::Render()
{
    bodyMesh->Render();
    sword->Render();
    __super::Render();
}

void Traveler::GUIRender()
{
    ImGui::DragInt("Swordtransform",&sword_transform,1.0f,0,199);
    ImGui::DragFloat("HP", &HP, 1.0f, 0, 100);
    string def = HPbar->GetLocalScale();
    ImGui::Text(def.c_str());

    bodyMesh->GUIRender();
    sword->GUIRender();
    sword->GetCollider()->GUIRender();
    __super::Render();
}

void Traveler::PostRender()
{
   // hpbar->Render();
}

void Traveler::SetMonster()
{
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

void Traveler::Control()
{
    if (curState == ATTACK) return;

    bool isMoveZ = false;
    bool isMoveX = false;

    if (KEY->Press('W'))
    {
        veloctiy.z += accelation * DELTA;
        isMoveZ = true;
    }

    if (KEY->Press('S'))
    {
        veloctiy.z -= accelation * DELTA;
        isMoveZ = true;
    }

    if (KEY->Press('D'))
    {
        veloctiy.x += accelation * DELTA;
        isMoveX = true;
    }

    if (KEY->Press('A'))
    {
        veloctiy.x -= accelation * DELTA;
        isMoveX = true;
    }

    if (veloctiy.Length() > 1.0f)
        veloctiy.Normalized();

    if (!isMoveX)
        veloctiy.x = MATH->Lerp(veloctiy.x, 0.0f, deceleration * DELTA);

    if (!isMoveZ)
        veloctiy.z = MATH->Lerp(veloctiy.z, 0.0f, deceleration * DELTA);

    float rot;
    if (Mouse::Get()->GetIsSetMouseHold())
        rot = rotSpeed * 3;
    else
        rot = rotSpeed;

    Rotate(Vector3::Up() * Mouse::Get()->GetMoveValue().x * rot * DELTA);
    CAM->Rotate(Vector3::Left() * Mouse::Get()->GetMoveValue().y * rot * DELTA);
}

void Traveler::Move()
{
    if (curState == ATTACK) return;

    Matrix rotY = XMMatrixRotationY(localRotation.y);
    Vector3 direction = XMVector3TransformNormal(veloctiy, rotY);

    Translate(direction * moveSpeed * DELTA);
}

void Traveler::SetAction()
{
    if (curState == ATTACK) return;

    if (veloctiy.z > EPSILON)
        SetState(RUN_F);
    else if (veloctiy.z < -EPSILON)
        SetState(RUN_B);
    else if (veloctiy.x > EPSILON)
        SetState(RUN_R);
    else if (veloctiy.x < -EPSILON)
        SetState(RUN_L);
    else
        SetState(IDLE);
}

void Traveler::ActiveSowdCollider()
{
    sword->GetCollider()->SetActive(true);
}

void Traveler::InactiveSowdCollider()
{
    sword->GetCollider()->SetActive(false);

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
    bodyMesh->ReadClip("Back_Walk");
    bodyMesh->ReadClip("Right_Walk");
    bodyMesh->ReadClip("Left_Walk");
    bodyMesh->ReadClip("Attack");
    bodyMesh->CreateTexture();

    bodyMesh->GetClip(ATTACK)->SetEvent(bind(&Traveler::EndAttack, this), 0.8f);
    bodyMesh->GetClip(ATTACK)->SetEvent(bind(&Traveler::ActiveSowdCollider, this), 0.3f);
    bodyMesh->GetClip(ATTACK)->SetEvent(bind(&Traveler::InactiveSowdCollider, this), 0.45f);
}

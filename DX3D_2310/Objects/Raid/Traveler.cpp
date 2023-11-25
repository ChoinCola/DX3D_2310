#include "Framework.h"
#include "Traveler.h"

Traveler::Traveler()
{
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
    bodyMesh->GUIRender();
    sword->GUIRender();
    sword->GetCollider()->GUIRender();
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

    Rotate(Vector3::Up() * Mouse::Get()->GetMoveValue().x * rotSpeed * DELTA);
    CAM->Rotate(Vector3::Left() * Mouse::Get()->GetMoveValue().y * rotSpeed * DELTA);
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

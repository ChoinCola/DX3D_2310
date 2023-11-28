#include "Framework.h"
#include "TopPlayer.h"

TopPlayer::TopPlayer()
{
    SetTag("TopPlayer");
    Load();
}

TopPlayer::~TopPlayer()
{
}

void TopPlayer::Update()
{
    UpdateWorld();
    Move();
}

void TopPlayer::Render()
{
    __super::Render();
}

void TopPlayer::GUIRender()
{
}

void TopPlayer::CamMove()
{
}

void TopPlayer::Move()
{
    if (state == ATTACK) return;

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

    Matrix rotY = XMMatrixRotationY(localRotation.y);
    Vector3 direction = XMVector3TransformNormal(veloctiy, rotY);

    Translate(direction * moveSpeed * DELTA);
}

void TopPlayer::UpdateUppos()
{
}

void TopPlayer::SetOnTerrain(Terrain* input)
{
}

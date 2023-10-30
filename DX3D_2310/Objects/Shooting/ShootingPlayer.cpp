#include "Framework.h"

ShootingPlayer::ShootingPlayer()
{
	cursor = new Quad(L"Textures/UI/cursor.png");
	cursor->SetLocalPosition({ CENTER_X, CENTER_Y, 0.0f });
	cursor->UpdateWorld();
}

ShootingPlayer::~ShootingPlayer()
{
	delete cursor;
}

void ShootingPlayer::Update()
{
	Move();
	UpdateWorld();
}

void ShootingPlayer::PostRender()
{
	cursor->Render();
}

void ShootingPlayer::Move()
{
	if (KEY->Press('W'))
	{
		Vector3 forward = GetForward();
		forward.y = 0;
		Translate(forward.GetNormalized() * moveSpeed * DELTA);
	}

	Vector3 delta = Mouse::Get()->GetMoveValue();

	Rotate(Vector3::Up() * delta.x * rotSpeed * DELTA);
	Rotate(Vector3::Left() * -delta.y * rotSpeed * DELTA);

	localPosition.y = terrain->GetOnGrondPosition(localPosition).y + Radius();

	CAM->SetLocalPosition(localPosition);
	CAM->SetLocalRotation(localRotation);
}

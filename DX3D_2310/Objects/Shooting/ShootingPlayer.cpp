#include "Framework.h"

ShootingPlayer::ShootingPlayer(Vector3 pos)
{
	Frontpos = new Quad();
	Frontpos->SetParent(this);
	Frontpos->SetLocalPosition(Vector3(100, 0, 0));
	//cursor = new Quad(L"Textures/UI/cursor.png");
	cursor = new Quad(Float2(100,100));
	cursor->SetLocalPosition({ CENTER_X, CENTER_Y, 0.0f });
	cursor->UpdateWorld();
	SetLocalPosition(pos);
}

ShootingPlayer::~ShootingPlayer()
{
	delete cursor;
}

void ShootingPlayer::Update()
{
	Move();
	UpdateWorld();
	// ī�޶��� ��ġ�� lightBuffer�� position�� �Ҵ�
	
	Vector3 cameraFront = Environment::Get()->GetMainCamera()->GetForward();
	cameraFront.Normalized();
	
	Environment::Get()->GetLightBuffer()->GetData()->lights.position = 
		Environment::Get()->GetMainCamera()->GetLocalPosition() + Vector3(0, 1, 0);

	// ī�޶��� ���� ���� ���͸� ���ϰ� ����ȭ

	//cameraFront = cameraFront * -1;
	// lightBuffer�� direction�� ī�޶��� ���� ������ �Ҵ�
	Environment::Get()->GetLightBuffer()->GetData()->lights.direction = cameraFront + Vector3(0, 0.001, 0);
}

void ShootingPlayer::PostRender()
{
	cursor->Render();
}

void ShootingPlayer::Move()
{
	Vector3 MoveVector;
	MoveVector.Zero();

	if (KEY->Press('W'))
	{
		Vector3 forward = GetForward();
		forward.y = 0;
		MoveVector += forward.GetNormalized() * moveSpeed * DELTA;
	}

	if (KEY->Press('S'))
	{
		Vector3 forward = GetBack();
		forward.y = 0;
		MoveVector += forward.GetNormalized() * moveSpeed * DELTA;
	}	

	if (KEY->Press('A'))
	{
		Vector3 forward = GetLeft();
		forward.y = 0;
		MoveVector += forward.GetNormalized() * moveSpeed * DELTA;
	}	

	if (KEY->Press('D'))
	{
		Vector3 forward = GetRight();
		forward.y = 0;
		MoveVector += forward.GetNormalized() * moveSpeed * DELTA;
	}

	Vector3 set1 = terrain->GetOnGrondPosition(GetLocalPosition() + MoveVector);
	Vector3 set2 = GetLocalPosition();

	Uppos = set1.y;

	Vector3 slide = (set2 - set1);
	slide.Normalized();
	MoveVector += slide * (Uppos * Uppos) * 5;

	Translate(MoveVector);
	Vector3 delta = Mouse::Get()->GetMoveValue();

	Rotate(Vector3::Up() * delta.x * rotSpeed * DELTA);
	Rotate(Vector3::Left() * -delta.y * rotSpeed * DELTA);

	localPosition.y = terrain->GetOnGrondPosition(localPosition, Vector3(0, Radius(), 0)).y;

	CAM->SetLocalPosition(localPosition);
	CAM->SetLocalRotation(localRotation);
}

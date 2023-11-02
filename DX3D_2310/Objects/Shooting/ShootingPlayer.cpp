#include "Framework.h"

ShootingPlayer::ShootingPlayer(Vector3 pos) 
	: SphereCollider()
{
	//cursor = new Quad(L"Textures/UI/cursor.png");
	cursor = new Quad(Float2(100,100));
	cursor->SetLocalPosition({ CENTER_X, CENTER_Y, 0.0f });
	cursor->GetMaterial()->SetDiffuseMap(L"Textures/UI/cursor.png");
	cursor->GetMaterial()->SetShader(L"Basic/Texture.hlsl");
	cursor->UpdateWorld();

	lightBar = new Quad(Float2(1000, 10));
	lightBar->SetLocalPosition({ CENTER_X, WIN_HEIGHT - 10, 0.0f });
	lightBar->GetMaterial()->SetDiffuseMap(L"Textures/Colors/Gray.png");
	lightBar->GetMaterial()->SetShader(L"Basic/Texture.hlsl");
	lightBar->UpdateWorld();

	light = Environment::Get()->GetLight(0);
	SetLocalPosition(pos);
	SetLight();
}

ShootingPlayer::~ShootingPlayer()
{
	delete cursor;
}

void ShootingPlayer::Update()
{
	UpdateLightBat();
	Move();
	UpdateWorld();
	// 카메라의 위치를 lightBuffer의 position에 할당
}

void ShootingPlayer::GUIRender()
{
	__super::GUIRender();
}

void ShootingPlayer::PostRender()
{
	cursor->Render();
	lightBar->Render();
}

void ShootingPlayer::Render()
{
	SetRender();

	mesh->Draw(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
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

void ShootingPlayer::SetLight()
{
	light->isActive = true;
	light->range = 70;
	light->inner = 83;
	light->outer = 29;
	light->attentionIntensity = 1;
	light->type = 2;
}

void ShootingPlayer::UpdateLightBat()
{
	Vector3 cameraFront = Environment::Get()->GetMainCamera()->GetForward();
	cameraFront.Normalized();
	light->position = Environment::Get()->GetMainCamera()->GetLocalPosition();
	light->direction = cameraFront + Vector3(0, 0.001, 0);
	//float Lightouter = 29;
	//float Lightinner = 83;

	//if (LightBat > 30) {
	//	light->outer = Lightouter * Sigmoid(LightBat, 0.2f, 10.0);
	//	light->inner = Lightinner * Sigmoid(LightBat, 0.2f, 10.0);
	//}
	//else
	//{
	//	if (lightBatdelta < 360)
	//		lightBatdelta += DELTA * 20;
	//	else
	//		lightBatdelta = 0;

	//	light->outer = Lightouter * Sigmoid(LightBat, 0.2f, 10.0) * (sin(lightBatdelta));
	//	light->inner = Lightinner * Sigmoid(LightBat, 0.2f, 10.0);
	//}
	//lightBar->SetLocalScale(Vector3(LightBat * 0.01, 1, 1));
	//lightBar->UpdateWorld();

	//if (LightBat > 0) {
	//	LightBat -= DELTA * 10 ;
	//}
	//else
	//{
	//	light->isActive = false;
	//}
}

float ShootingPlayer::Sigmoid(float x, float k, float x0)
{
	return 1.0 / (1.0 + exp(-k * (x - x0)));
}

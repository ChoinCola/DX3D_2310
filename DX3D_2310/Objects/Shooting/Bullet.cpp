#include "Framework.h"

Bullet::Bullet(Vector3 Pos)
{
	ItemType = Item::Ammo;
	SetLocalPosition(Pos);
	Object = new Cube();
	Object->GetMaterial()->SetDiffuseMap(L"Textures/Landscape/Ammo.png");

	Object->SetLocalPosition(Pos);

	light = Environment::Get()->SetLight();
	Lightsetting();

}

Bullet::~Bullet()
{
	SAFE_DELETE(Object);
}

void Bullet::Update()
{
	// 위아래 요잉하는함수
	if (Yoing >= 360)
		Yoing = 0;
	Yoing += DELTA;
	Object->Translate(Vector3(0, sin(Yoing) * 0.00001, 0));

	// 회전하는 함수.
	Object->Rotate(Vector3(0, RotationSpeed * DELTA, 0));
	Object->UpdateWorld();
}

void Bullet::Render()
{
	if (IsRender) {
		Object->Render();
		UpdateWorld();
	}
}

void Bullet::GetItem(int& player)
{
	player++;
	IsRender = false;
	Environment::Get()->GetLight(light - 1)->isActive = false;
}

void Bullet::Lightsetting()
{
	LightBuffer::Light* lightset = Environment::Get()->GetLight(light-1);

	lightset->isActive = true;

	lightset->position = GetLocalPosition() + Vector3(0, 2, 0);
	Vector3 direct = GetLocalPosition() - lightset->position;
	direct.Normalized();
	lightset->direction = direct;
	lightset->type = 2;
	lightset->range = 20;
	lightset->color = { 0, 0.3, 0.7, 1 };
	lightset->attentionIntensity = 0.5;
	lightset->outer = 33;
	lightset->inner = 74;
}




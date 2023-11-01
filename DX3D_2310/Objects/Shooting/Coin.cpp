#include "Framework.h"

int Coin::Coincount = 0;

Coin::Coin(Vector3 Pos) 
{
	ItemType = Item::Gold;
	SetLocalPosition(Pos);
	Object = new Cube();
	Object->GetMaterial()->SetDiffuseMap(L"Textures/Landscape/Gold.png");
	Object->GetMaterial()->SetNormalMap(L"Textures/Landscape/GoldNormal.png");

	Object->SetLocalPosition(Pos);
	Coincount++;
}

Coin::~Coin()
{
	SAFE_DELETE(Object);
}

void Coin::Update()
{
	// 위아래 요잉하는함수
	if (Yoing >= 360)
		Yoing = 0;
	Yoing += DELTA;
	Object->Translate(Vector3(0, sin(Yoing) * 0.00001, 0));

	// 회전하는 함수.
	Object->Rotate(Vector3(0, RotationSpeed * DELTA, 0));
	Object->UpdateWorld();
	UpdateWorld();
}

void Coin::Render()
{
	if (IsRender) {
		Object->Render();
		UpdateWorld();
	}
}

void Coin::GetItem(int& player)
{
	player++;
	IsRender = false;
}

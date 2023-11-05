#include "Framework.h"
#include "ItemBox.h"

ItemBox::ItemBox(Float2 Boxsize, string name)
	: teg(name)
{
	itemUI = new Quad(Boxsize);
	itemUI->GetMaterial()->SetDiffuseMap(L"Textures/Colors/Black.png");
	itemUI->SetLocalPosition(Vector3(WIN_WIDTH * 0.5, WIN_HEIGHT * 0.5));

	CloaseButton = new Quad(Float2(100, 100));
	CloaseButton->GetMaterial()->SetDiffuseMap(L"Textures/Colors/Yellow.png");
	CloaseButton->SetLocalPosition(itemUI->GetLocalPosition() - Vector3(Boxsize.x * 0.5,-Boxsize.y * 0.5));

	UpBar = new Quad(Float2(Boxsize.x, 10));
	UpBar->GetMaterial()->SetDiffuseMap(L"Textures/Colors/Green.png");
	UpBar->SetLocalPosition(itemUI->GetLocalPosition() - Vector3(0, -Boxsize.y * 0.5));

	number = ItemBoxnum;
	ItemBoxnum++;
}

ItemBox::~ItemBox()
{
	SAFE_DELETE(itemUI);
}

void ItemBox::RenderUI()
{
	if (!IsCloase) {
		itemUI->Render();
		CloaseButton->Render();
		UpBar->Render();
	}
}

void ItemBox::Update()
{
	if (!IsCloase) {
		CloaseButton->UpdateWorld();
		itemUI->UpdateWorld();
		UpBar->UpdateWorld();
	}
}

void ItemBox::DeleteItem()
{

}

void ItemBox::ChackItem()
{

}

void ItemBox::UseItem()
{
}

bool ItemBox::CloaseCollisionCheck()
{
	if (CloaseButton->CollisionChack(Mouse::Get()->GetPosition()) && Mouse::Get()->Down(0))
		return true;
}

void ItemBox::Load()
{
	itemUI->Load();
}

void ItemBox::Save()
{
	itemUI->Save();
}

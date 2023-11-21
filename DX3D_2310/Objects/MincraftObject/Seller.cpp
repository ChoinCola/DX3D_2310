#include "Framework.h"

Seller::Seller()
{
	localPosition = { 0, 4, 0 };
	ui = new ShopUI(L"Textures/UI/MineCraftUI/ShopUI.png");
	model = new Model("Traveler");
	model->SetParent(this);
	model->SetLocalScale(Vector3(0.001, 0.001, 0.001));

	SetTag("Seller");
	model->SetTag("SellerModel");

	Observer::Get()->AddParamEvent("SellerCollision", bind(&Seller::ChackOpenUi, this, placeholders::_1));

	Load();
	model->Load();
}

Seller::~Seller()
{
	Save();
	model->Save();
	SAFE_DELETE(ui);
}

void Seller::Update()
{
	model->UpdateWorld();
	ui->Update();
	BlockManager::Get()->CollisionBlockToPlayer(*this);
	UpdateWorld();
}

void Seller::GUIRender()
{
	__super::GUIRender();
	ui->GUIRender();
	model->GUIRender();
}

void Seller::PostRender()
{
	ui->PostRender();
}

void Seller::Render()
{
	__super::Render();
	model->Render();
}

void Seller::ChackOpenUi(void* input)
{
	Collider* col = static_cast<Collider*>(input);

	if (Onlyone && Mouse::Get()->Down(0) && IsCollision(col)) {
		ui->IsOpenUI(true);
		Onlyone = false;
	}
	else if(!IsCollision(col))
	{
		ui->IsOpenUI(false);
		Onlyone = true;
	}
}

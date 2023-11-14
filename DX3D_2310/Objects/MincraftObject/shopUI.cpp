#include "Framework.h"

ShopUI::ShopUI(wstring textureFile)
	: Quad(textureFile)
{
	SetLocalScale(Vector3(2, 2, 2));
	SetActive(false);
	IsRender();
	UINT Box_Width = 3;
	UINT Box_Hight = 8;
	SetTag("ShopUI");

	moneypos = new Transform();
	moneypos->SetParent(this);
	moneypos->SetTag("MoneyPos");
	moneypos->Load();
	for (int i = 0; i < Box_Hight; i++)
	{
		for (int j = 0; j < Box_Width; j++)
		{
			Itemmap[i * Box_Width + j] = new InvenBlock(ItemmapBase + Vector3(j * 18, i * 18, 0), this);
			Itemmap[i * Box_Width + j]->SetHasPlayer();
			Itemmap[i * Box_Width + j]->SetCanInput();
			Itemmap[i * Box_Width + j]->SetParent(this);

			Event def = bind(&InvenBlock::PopMouse, Itemmap[i * Box_Width + j]);
			Itemmap[i * Box_Width + j]->SetEvent(def);

			def = bind(&InvenBlock::InsertMouseFrominven, Itemmap[i * Box_Width + j]);
			Itemmap[i * Box_Width + j]->SetUpEvent(def);

			Itemmap[i * Box_Width + j]->IsRender();
			Itemmap[i * Box_Width + j]->SetActive(false);
			UIManager::Get()->AddUI(Itemmap[i * Box_Width + j]);
		}
	}

	for (int i = 0; i < Box_Hight; i++)
	{
		for (int j = 0; j < Box_Width; j++)
		{
			Buymap[i * Box_Width + j] = new InvenBlock(BuymapBase + Vector3(j * 18, i * 18, 0), this);
			Buymap[i * Box_Width + j]->SetParent(this);

			Event def = bind(&InvenBlock::PopMouse, Buymap[i * Box_Width + j]);
			Buymap[i * Box_Width + j]->SetEvent(def);

			def = bind(&InvenBlock::InsertMouseFrominven, Buymap[i * Box_Width + j]);
			Buymap[i * Box_Width + j]->SetUpEvent(def);

			Buymap[i * Box_Width + j]->IsRender();
			Buymap[i * Box_Width + j]->SetActive(false);
			UIManager::Get()->AddUI(Buymap[i * Box_Width + j]);
		}
	}

	Load();
}

ShopUI::~ShopUI()
{
	Save();
	moneypos->Save();
}

void ShopUI::Update()
{
	Under_inventoryViewUpdate();

	if (MouseBag::Get()->GetBlock() == nullptr) {
		if (!isDrag && Mouse::Get()->Down(0) && CollisionChack(Mouse::Get()->GetPosition()))
		{
			isDrag = true;
			dragoffset = GetGlobalPosition() - Mouse::Get()->GetPosition();
		}

		if (isDrag && Mouse::Get()->Press(0))
		{
			SetLocalPosition(Mouse::Get()->GetPosition() + dragoffset);
		}
	}
	if (Mouse::Get()->Up(0))
		isDrag = false;

	UpdateWorld();
	InventoryposUpdate();
	moneypos->UpdateWorld();
}

void ShopUI::PostRender()
{
	Quad::Render();
	InventoryRender();
}

void ShopUI::GUIRender()
{
	__super::GUIRender();
	moneypos->GUIRender();
}

void ShopUI::insertblock(Block* block)
{
	FOR(24)
	{
		if (Buymap[i]->InsertBlock(block))
			return;
		if (Itemmap[i]->InsertBlock(block))
			return;
	}
}

void ShopUI::IsOpenUI(bool input)
{
	if(input != GetRender())
		IsRender();

	SetActive(!GetRender());
}

void ShopUI::InventoryRender()
{
	if (GetRender()) {
		FOR(24)
		{
			Itemmap[i]->PostRender();
			Buymap[i]->PostRender();
		}

		Font::Get()->RenderText(to_string(Buymapmoney), moneypos->GetGlobalPosition());

		Vector3 def(-80, 0, 0);
		if(Buymapmoney < 0)
			Font::Get()->RenderText("-", moneypos->GetGlobalPosition() + def);
		else
			Font::Get()->RenderText("+", moneypos->GetGlobalPosition() + def);

	}
}

void ShopUI::InventoryposUpdate()
{
	int moneyResult = 0;
	FOR(24) 
	{
		Itemmap[i]->Update();
		Buymap[i]->Update();
		if (Buymap[i]->GetBlock() != nullptr)
		{
			if (Buymap[i]->GetHasPlayer())
				moneyResult += Buymap[i]->GetBlock()->GetBlockData().cash * Buymap[i]->GetCount();
			else
				moneyResult -= Buymap[i]->GetBlock()->GetBlockData().cash * Buymap[i]->GetCount();
		}
	}

	Buymapmoney = moneyResult;
}

void ShopUI::Under_inventoryViewUpdate()
{
}

void ShopUI::Under_inventoryViewRender()
{
}

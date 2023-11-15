#include "Framework.h"

ShopUI::ShopUI(wstring textureFile)
	: Quad(textureFile)
{
	Block* gold = new Block(BlockDataManager::Get()->GetObjectDatas(202));
	delete gold;
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
	Block* inputblock = new Block(BlockDataManager::Get()->GetObjectDatas(201));
	insertItemmap(inputblock);

	TradeButton = new Button(L"Textures/UI/MineCraftUI/Trade.png");
	TradeButton->SetParent(this);
	TradeButton->SetEvent(bind(&ShopUI::Trade, this));
	TradeButton->SetOverEvent(bind(&ShopUI::Rotate_TradeOutline, this));

	TradeOutline = new Quad(L"Textures/UI/MineCraftUI/Trade_Outline.png");
	TradeOutline->SetParent(TradeButton);
	TradeButton->SetTag("TradeButton");
	TradeButton->Load();

	Load();
}

ShopUI::~ShopUI()
{
	Save();
	moneypos->Save();
	TradeButton->Save();

	SAFE_DELETE(moneypos);
	SAFE_DELETE(TradeButton);
	SAFE_DELETE(TradeOutline);
}

void ShopUI::Update()
{

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
	TradeButton->GUIRender();
}

void ShopUI::insertItemmap(Block* block)
{
	FOR(24)
	{
		if (Itemmap[i]->InsertBlock(block, 1, false))
			return;
	}

	return;
}

void ShopUI::insertItemmap(InvenBlock* block)
{
	FOR(24)
	{
		if (Itemmap[i]->InsertBlock(block->GetBlock(), block->GetCount(), false))
		{
			block->SetBlock(nullptr);
			block->SetCount(0);
			return;
		}
			
	}

	return;
}

void ShopUI::insertBuymap(Block* block, UINT count, bool property)
{
	FOR(24)
	{
		if (Buymap[i]->InsertBlock(block, count, property)) {
			if (Buymap[i]->GetHasPlayer() == false) Buymap[i]->SetHasPlayer();
		}
			return;
	}

	// 못넣으면 삭제함.
	delete block;
	return;
}

void ShopUI::Trade()
{
	int nowmoney = Buymapmoney;
	int tradeitemmoney = 0;
	int mygold = 0;

	if (nowmoney < 0) return;

	FOR(24)
	{
		if (Buymap[i]->GetBlock() != nullptr && Buymap[i]->GetBlock()->GetBlockData().itemtype == "gold")
			mygold += Buymap[i]->GetBlock()->GetBlockData().cash * Buymap[i]->GetCount();

		if (Buymap[i]->GetHasPlayer() == true && Buymap[i]->GetBlock() != nullptr)
		{
			Buymap[i]->SetHasPlayer();
			insertItemmap(Buymap[i]);
		}
		else if (Buymap[i]->GetHasPlayer() == false && Buymap[i]->GetBlock() != nullptr) {
			if (Buymap[i]->GetHasPlayer() == false) Buymap[i]->SetHasPlayer();
			tradeitemmoney += Buymap[i]->GetBlock()->GetBlockData().cash * Buymap[i]->GetCount();
		}

	}
	//nowmoney -= mygold;

	int resultgold = nowmoney;

	if (resultgold > 0) {
		Block* gold = new Block(BlockDataManager::Get()->GetObjectDatas(202));
		insertBuymap(gold, resultgold, true);
	}
}

void ShopUI::IsOpenUI(bool input)
{
	if(input != GetRender())
		IsRender();

	SetActive(!GetRender());
}

void ShopUI::Rotate_TradeOutline()
{
	TradeOutline->Rotate(Vector3::Forward() * DELTA * 5);
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

		TradeButtonRender();
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
		else
		{
			if (Buymap[i]->GetHasPlayer() == true) Buymap[i]->SetHasPlayer();
		}
	}

	Buymapmoney = moneyResult;
	TradeButtonUpdate();
}

void ShopUI::TradeButtonRender()
{
	TradeButton->Render();
	TradeOutline->Render();
}

void ShopUI::TradeButtonUpdate()
{
	TradeButton->Update();
	TradeOutline->UpdateWorld();
}

InvenBlock* ShopUI::GetInvenBlockdataInitemmap(int num)
{
	return nullptr;
}

#include "Framework.h"
#include "InventoryUI.h"

InventoryUI::InventoryUI(wstring textureFile)
	: Quad(textureFile)
{
	SetActive(false);

	Under_inventoryView.resize(9);
	FOR(9)
	{
		Under_inventorymap[i] = new InvenBlock(inventoryBaseDown + Vector3(i * 18, 0, 0), this);

		Event isDrag = bind(&InventoryUI::SetDrag, this);
		Under_inventorymap[i]->SetEvent(isDrag);
		
		Event nondef = bind(&InventoryUI::SetDrag, this);
		Under_inventorymap[i]->SetEvent(nondef);

		ParamEvent def = bind(&InvenBlock::PopMouse, Under_inventorymap[i], placeholders::_1);
		Under_inventorymap[i]->SetPramEvnet(def, &MouseBag);

		def = bind(&InvenBlock::InsertMouse, Under_inventorymap[i], placeholders::_1);
		Under_inventorymap[i]->SetUpPramEvnet(def, &MouseBag);

		Under_inventorymap[i]->IsRender();

		Quad* insertquad = new Quad(L"Textures/UI/Blocks/block0.png");
		insertquad->SetLocalPosition(Under_inventorymapBaseDown + Vector3(i * 40, 0, 0));
		insertquad->UpdateWorld();

		Under_inventoryView[i] = insertquad;
		Under_inventoryView[i]->IsRender();
		Under_inventoryView[i]->SetActive(false);
	}

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			inventorymap[i * 9 + j] = new InvenBlock(inventoryBase + Vector3(j * 18, i * 18, 0), this);

			Event nondef = bind(&InventoryUI::SetDrag, this);
			inventorymap[i * 9 + j]->SetEvent(nondef);

			ParamEvent def = bind(&InvenBlock::PopMouse, inventorymap[i * 9 + j], placeholders::_1);
			inventorymap[i * 9 + j]->SetPramEvnet(def, Mouse::Get()->GetPositionOrigin());

			def = bind(&InvenBlock::InsertMouse, inventorymap[i * 9 + j], placeholders::_1);
			Under_inventorymap[i]->SetUpPramEvnet(def, &MouseBag);

			inventorymap[i * 9 + j]->IsRender();
			inventorymap[i * 9 + j]->SetActive(false);
		}
	}

	SetTag("InventoryPanal");
	Load();
}

InventoryUI::~InventoryUI()
{
	Save();
}

void InventoryUI::Update()
{
	Under_inventoryViewUpdate();

	if (!isDrag && Mouse::Get()->Down(0) && CollisionChack(Mouse::Get()->GetPosition()))
	{
		isDrag = true;
		offset = GetGlobalPosition() - Mouse::Get()->GetPosition();
	}

	if (isDrag && Mouse::Get()->Press(0))
	{
		SetLocalPosition(Mouse::Get()->GetPosition() + offset);
	}

	if (Mouse::Get()->Up(0))
		isDrag = false;

	UpdateWorld();
	InventoryposUpdate();
	Update_MouseBag();
}

void InventoryUI::PostRender()
{
	Quad::Render();
	InventoryRender();
	Under_inventoryViewRender();
	PostRender_MouseBag();
}

void InventoryUI::insertblock(Block* block)
{
	FOR(9)
	{
		if (Under_inventorymap[i]->InsertBlock(block))
			return;
	}

	FOR(27)
	{
		if (inventorymap[i]->InsertBlock(block))
			return;
	}
}

void InventoryUI::InventoryRender()
{
	FOR(9)
		Under_inventorymap[i]->PostRender();

	FOR(27)
		inventorymap[i]->PostRender();

}

void InventoryUI::InventoryposUpdate()
{
	FOR(9) {
		Under_inventorymap[i]->Update();
	}

	FOR(27) {
		inventorymap[i]->Update();
	}
}

void InventoryUI::Under_inventoryViewUpdate()
{
	FOR(9)
	{
		if (Under_inventorymap[i]->GetBlock() != nullptr)
		{
			if (Under_inventoryView[i]->GetRender() == false)
				Under_inventoryView[i]->IsRender();

			wstring def = ToWString(Under_inventorymap[i]->GetBlock()->GetBlockData().modelname);
			Under_inventoryView[i]->GetMaterial()->SetDiffuseMap(L"Textures/UI/Blocks/" + def + L".png");
		}
		else
		{
			if (Under_inventoryView[i]->GetRender() == true)
				Under_inventoryView[i]->IsRender();
		}
	}
}

void InventoryUI::Under_inventoryViewRender()
{
	FOR(9)
	{
		if (Under_inventorymap[i]->GetBlock() != nullptr)
		{
			Under_inventoryView[i]->Render();
			string co = to_string(Under_inventorymap[i]->GetCount());
			Float2 pos = Float2(Under_inventoryView[i]->GetLocalPosition().x + 18, Under_inventoryView[i]->GetLocalPosition().y - 8);
			Font::Get()->RenderText(co, pos);
		}
	}
}

void InventoryUI::Update_MouseBag()
{
	if (MouseBag.second.second != nullptr)
	{
		//MouseBag.second.second->SetParent(nullptr);
		MouseBag.second.second->GetInventoryModel()->SetLocalPosition(Mouse::Get()->GetPosition());
		MouseBag.second.second->Update();
	}
}

void InventoryUI::PostRender_MouseBag()
{
	if (MouseBag.second.second != nullptr)
	{
		string co = to_string(MouseBag.second.first);
		Float2 pos = Float2(MouseBag.second.second->GetLocalPosition().x + 18, MouseBag.second.second->GetLocalPosition().y - 8);
		Font::Get()->RenderText(co, pos);

		MouseBag.second.second->InventoryRender();
	}
}

void InventoryUI::Drop()
{
	// Drop
}
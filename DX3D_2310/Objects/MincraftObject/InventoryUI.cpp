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
		Under_inventorymap[i]->SetParent(this);

		Event def = bind(&InvenBlock::PopMouse, Under_inventorymap[i]);
		Under_inventorymap[i]->SetEvent(def);

		def = bind(&InvenBlock::InsertMouseFrominven, Under_inventorymap[i]);
		Under_inventorymap[i]->SetUpEvent(def);

		Under_inventorymap[i]->IsRender();

		Quad* insertquad = new Quad(L"Textures/UI/Blocks/block0.png");
		insertquad->SetLocalPosition(Under_inventorymapBaseDown + Vector3(i * 40, 0, 0));
		insertquad->UpdateWorld();

		Under_inventoryView[i] = insertquad;
		Under_inventoryView[i]->IsRender();
		Under_inventoryView[i]->SetActive(false);
		
		UIManager::Get()->AddUI(Under_inventoryView[i]);
	}

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			inventorymap[i * 9 + j] = new InvenBlock(inventoryBase + Vector3(j * 18, i * 18, 0), this);
			inventorymap[i * 9 + j]->SetParent(this);

			Event def = bind(&InvenBlock::PopMouse, inventorymap[i * 9 + j]);
			inventorymap[i * 9 + j]->SetEvent(def);

			def = bind(&InvenBlock::InsertMouseFrominven, inventorymap[i * 9 + j]);
			inventorymap[i * 9 + j]->SetUpEvent(def);

			inventorymap[i * 9 + j]->IsRender();
			inventorymap[i * 9 + j]->SetActive(false);
			UIManager::Get()->AddUI(inventorymap[i * 9 + j]);
		}
	}

	Load();
}

InventoryUI::~InventoryUI()
{
	Save();
}

void InventoryUI::Update()
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
}

void InventoryUI::PostRender()
{
	Quad::Render();
	InventoryRender();
	Under_inventoryViewRender();
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
	if (GetRender()) {
		FOR(9)
			Under_inventorymap[i]->PostRender();

		FOR(27)
			inventorymap[i]->PostRender();
	}

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
#include "Framework.h"
#include "MineCraftUI.h"

MineCraftUI::MineCraftUI()
{
	{
		//cursor
		UIVector.emplace_back(new Quad(L"Textures/UI/Cursor.png"));
		UIVector.back()->SetLocalPosition({ CENTER_X, CENTER_Y, 0.01f });
		UIVector.back()->UpdateWorld();
		UIVector.back()->SetTag("Cursor");
	}

	{
		// UnderUI
		UIVector.emplace_back(new Quad(L"Textures/UI/MineCraftUI/Play_Under_Bar.png"));
		UIVector.back()->SetLocalPosition({ CENTER_X, 20, 0.0f });
		UIVector.back()->SetLocalScale(Vector3(2, 2, 2));
		UIVector.back()->UpdateWorld();
		UIVector.back()->SetTag("Bar");
		UnderInventory = UIVector.back();
	}

	{
		// SelectUI
		UIVector.emplace_back(new Quad(L"Textures/UI/MineCraftUI/Play_Under_Bar_Select.png"));
		SelectBasePosition = { CENTER_X - 160, 20, -0.001f };
		UIVector.back()->SetLocalPosition(SelectBasePosition);
		UIVector.back()->SetLocalScale(Vector3(2, 2, 2));
		UIVector.back()->UpdateWorld();
		UIVector.back()->SetTag("Select");

		Select = UIVector.back();
	}

	{
		// InventoryUI 한칸당 36씩 이동. 두배수니까 원래 사이즈는 18.
		// 좌상단 인벤토리 위치 : 496, 342
		// 좌하단 사용인벤토리 위치 : 496, 226
		inventory = new InventoryUI(L"Textures/UI/MineCraftUI/inventory.png");
	}
	// 데이터 시작시 인벤토리 Pos 정해줌.

}

MineCraftUI::~MineCraftUI()
{
	for (Quad* def : UIVector)
		delete def;
}

void MineCraftUI::Update()
{
	Under_UI();
	ViewInventory();
	Drag();
	Drop();
	MouseBagUpdate();

	for (Quad* def : UIVector)
		def->UpdateWorld();
	inventory->Update();
}

void MineCraftUI::PostRender()
{

	for (Quad* def : UIVector)
		def->Render();
	MouseBagRender();
	InventoryRender();
}

void MineCraftUI::GUIRender()
{
	for (Quad* def : UIVector)
		def->GUIRender();
}

void MineCraftUI::Mining()
{
	//Ray ray = CAM->ScreenPointToRay(Vector3(CENTER_X, CENTER_Y, 0));
	Ray ray = CAM->ScreenPointToRay(Mouse::Get()->GetPosition());

	Block* block = BlockManager::Get()->DeleteBlock(ray);

	if (block == nullptr)
		return;

	InsertInventory(block);
}

void MineCraftUI::Build()
{
	if (inventory->GetUnderinventory(nowselect).count == 0) return;
	else if (inventory->GetUnderinventory(nowselect).count > 0)
	{
		Block* block = new Block(inventory->GetUnderinventory(nowselect).block->GetBlockData());

		if (BlockManager::Get()->AddBlock(block))
			inventory->GetUnderinventory(nowselect).count--;
		else
			delete block;
	}

	if (inventory->GetUnderinventory(nowselect).count == 0) inventory->GetUnderinventory(nowselect).block = nullptr;
}

void MineCraftUI::Under_UI()
{
	FOR(9)
	{
		if (KEY->Down('1' + i)) {
			Select->SetLocalPosition(SelectBasePosition + Vector3(i * 40, 0, 0));
			nowselect = i;
		}
	}
}

void MineCraftUI::ViewInventory()
{
	if (KEY->Down('I'))
	{
		inventory->IsActive();
	}
}

void MineCraftUI::InventoryRender()
{
	FOR(9)
	{
		if (Under_inventorymap[i].block != nullptr) {
			Under_inventorymap[i].block->GetInventoryModel()->SetLocalPosition(Under_inventorymap[i].pos);
			Under_inventorymap[i].block->GetInventoryModel()->UpdateWorld();
			Under_inventorymap[i].block->GetInventoryModel()->Render();
		}
	}
}


void MineCraftUI::MouseBagUpdate()
{
	if (MouseBag.second.block != nullptr)
	{
		MouseBag.second.block->GetInventoryModel()->SetLocalPosition(Mouse::Get()->GetPosition() + Vector3(0, 0, -1)); 
		MouseBag.second.block->GetInventoryModel()->UpdateWorld();
	}
}

void MineCraftUI::MouseBagRender()
{
	if (MouseBag.second.block != nullptr)
	{
		MouseBag.second.block->GetInventoryModel()->Render();
	}
}


bool MineCraftUI::InsertInventory(Block* input)
{
	FOR(9)
	{
		if (Under_inventorymap[i].block == nullptr) {
			Under_inventorymap[i].count = 1;
			Under_inventorymap[i].block = input;
			return true;
		}
		else if (Under_inventorymap[i].block->GetBlockData().name == input->GetBlockData().name) {
			Under_inventorymap[i].count++;
			delete input;
			return true;
		}
	}

	FOR(27)
	{
		if (inventorymap[i].block == nullptr) {
			inventorymap[i].count = 1;
			inventorymap[i].block = input;
			return true;
		}
		else if (inventorymap[i].block->GetBlockData().name == input->GetBlockData().name) {
			inventorymap[i].count++;
			delete input;
			return true;
		}
	}

	return false;
}

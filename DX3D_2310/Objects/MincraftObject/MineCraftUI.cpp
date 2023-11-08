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
		UIVector.emplace_back(new Quad(L"Textures/UI/MineCraftUI/inventory.png"));
		UIVector.back()->SetLocalPosition({ CENTER_X, CENTER_Y, 0.0f });
		UIVector.back()->SetLocalScale(Vector3(2, 2, 2));
		UIVector.back()->UpdateWorld();
		UIVector.back()->SetTag("Inventory");

		Inventory = UIVector.back();
		Inventory->IsRender();
	}

	FOR(9)
	{
		Under_inventorymap[i].pos = Vector3(CENTER_X - 160, 20, 0) + Vector3(i * 40, 0, 0.01);
		Under_inventorymap[i].pos2 = Vector3(496, 226, 0) + Vector3(i * 36, 0, 0.01);
	}


	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			inventorymap[i*9 + j].pos = Vector3(496, 342, 0) + Vector3(j * 36, i * -36, 0.01);
		}
	}
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
}

void MineCraftUI::PostRender()
{
	MouseBagRender();
	InventoryRender();
	for (Quad* def : UIVector)
		def->Render();
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
	if (Under_inventorymap[nowselect].count == 0) return;
	else if (Under_inventorymap[nowselect].count > 0)
	{
		Block* block = new Block(Under_inventorymap[nowselect].block->Getname());

		if (BlockManager::Get()->AddBlock(block))
			Under_inventorymap[nowselect].count--;
		else
			delete block;
	}

	if (Under_inventorymap[nowselect].count == 0) Under_inventorymap[nowselect].block = nullptr;
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
		Inventory->IsRender();
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

	if (Inventory->GetRender()) {
		FOR(9)
		{
			if (Under_inventorymap[i].block != nullptr) {
				Under_inventorymap[i].block->GetInventoryModel()->SetLocalPosition(Under_inventorymap[i].pos2);
				Under_inventorymap[i].block->GetInventoryModel()->UpdateWorld();
				Under_inventorymap[i].block->GetInventoryModel()->Render();
			}
		}

		FOR(27)
		{
			if (inventorymap[i].block != nullptr) {
				inventorymap[i].block->GetInventoryModel()->SetLocalPosition(inventorymap[i].pos);
				inventorymap[i].block->GetInventoryModel()->UpdateWorld();
				inventorymap[i].block->GetInventoryModel()->Render();
			}
		}
	}

}

void MineCraftUI::Drag()
{
	// Drag 중이 아닐때만 동작함.
	// 밑 칸은 false, 윗칸은 true
	if (!isDrag)
	{
		FOR(9)
		{
			if (Under_inventorymap[i].block != nullptr) {
				// 마우스가 집으면
				if (Under_inventorymap[i].block->GetInventoryModel()->CollisionChack(Mouse::Get()->GetPosition()) && Mouse::Get()->Down(0))
				{
					// 첫 값에는 원래 포지션의 번호와 위치를 가져옴.
					MouseBag.first.first = i;
					MouseBag.first.second = false;

					// 값에는 그 블럭의 숫자와  그 블럭의 포인터를 가져옴.
					MouseBag.second = Under_inventorymap[i];

					// 원래있었던 데이터의 포인터는 null로 초기화해줌.
					Under_inventorymap[i].count = 0;
					Under_inventorymap[i].block = nullptr;
					isDrag = true;

					return; // 마우스가 집으면 끝
				}
			}

		}

		FOR(27)
		{
			if (inventorymap[i].block != nullptr) {
				// 마우스가 집으면
				if (inventorymap[i].block->GetInventoryModel()->CollisionChack(Mouse::Get()->GetPosition()) && Mouse::Get()->Down(0))
				{
					// 첫 값에는 원래 포지션의 번호와 위치를 가져옴.
					MouseBag.first.first = i;
					MouseBag.first.second = true;

					// 값에는 그 블럭의 숫자와  그 블럭의 포인터를 가져옴.
					MouseBag.second = inventorymap[i];

					// 원래있었던 데이터의 포인터는 null로 초기화해줌.

					inventorymap[i].count = 0;
					inventorymap[i].block = nullptr;
					isDrag = true;

					return; // 마우스가 집으면 끝
				}
			}
		}
	}


}

void MineCraftUI::Drop()
{
	// Drop
	if (!Mouse::Get()->Press(0) && !Mouse::Get()->Down(0) && isDrag) // 안누르고 있을시. 연산진행함. 드래그중일때도 포함.
	{
		// 일단 드래그는 끝이니까 끝이라고 알려줌
		isDrag = false;
		if (MouseBag.second.block != nullptr) // 가지고 온 bag이 nullptr이 아닐경우.
		{
			Vector3 nowpos = Mouse::Get()->GetPosition();

			// 인벤토리 창이 열려있는지 먼저 체크 인벤토리 창 안인지 확인.
			// 인벤토리창 안에 있을경우,
			if (Inventory->CollisionChack(nowpos))
			{
				//검사
				float minfloat = FLT_MAX;
				UINT insertpos;
				bool insert;

				FOR(27)
				{
					float len = (inventorymap[i].pos - nowpos).Length();
					if (minfloat > len && 
						(inventorymap[i].block == nullptr || inventorymap[i].block->Getname() == MouseBag.second.block->Getname())) {
						minfloat = len;
						insertpos = i;
						insert = true;
					}
				}

				FOR(9)
				{
					float len = (Under_inventorymap[i].pos2 - nowpos).Length();
					if (minfloat > len && 
					(Under_inventorymap[i].block == nullptr || Under_inventorymap[i].block->Getname() == MouseBag.second.block->Getname())) {
						minfloat = len;
						insertpos = i;
						insert = false;
					}
				}

				// 드롭시 가장 가까운 칸에 넣어줌.
				if (insert) {
					inventorymap[insertpos].count += MouseBag.second.count;
					inventorymap[insertpos].block = MouseBag.second.block;
					MouseBag.second.clear();
				}
				else {
					Under_inventorymap[insertpos].count += MouseBag.second.count;
					Under_inventorymap[insertpos].block = MouseBag.second.block;
					MouseBag.second.clear();
				}

				return;
			}

			if (UnderInventory->CollisionChack(nowpos))
			{
				float minfloat = FLT_MAX;
				UINT insertpos;
				FOR(9)
				{
					float len = (Under_inventorymap[i].pos - nowpos).Length();
					if (minfloat > len && 
						(Under_inventorymap[i].block == nullptr || Under_inventorymap[i].block->Getname() == MouseBag.second.block->Getname())) {
						minfloat = len;
						insertpos = i;
					}

				}

				Under_inventorymap[insertpos].count += MouseBag.second.count;
				Under_inventorymap[insertpos].block = MouseBag.second.block;
				MouseBag.second.clear();
				return;

			}
		}

		// 그 무엇에도 넣지 못했는데 드롭했다면 원래 위치로 귀환함.
		if (MouseBag.first.second)
			inventorymap[MouseBag.first.first] = MouseBag.second;
		else
			Under_inventorymap[MouseBag.first.first] = MouseBag.second;

		MouseBag.second.clear();
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
		else if (Under_inventorymap[i].block->Getname() == input->Getname()) {
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
		else if (inventorymap[i].block->Getname() == input->Getname()) {
			inventorymap[i].count++;
			delete input;
			return true;
		}
	}

	return false;
}

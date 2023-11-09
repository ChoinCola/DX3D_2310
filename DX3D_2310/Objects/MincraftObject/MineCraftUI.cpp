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
		// InventoryUI ��ĭ�� 36�� �̵�. �ι���ϱ� ���� ������� 18.
		// �»�� �κ��丮 ��ġ : 496, 342
		// ���ϴ� ����κ��丮 ��ġ : 496, 226
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
		Block* block = new Block(Under_inventorymap[nowselect].block->GetBlockData());

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
	// Drag ���� �ƴҶ��� ������.
	// �� ĭ�� false, ��ĭ�� true
	if (!isDrag)
	{
		FOR(9)
		{
			if (Under_inventorymap[i].block != nullptr) {
				// ���콺�� ������
				if (Under_inventorymap[i].block->GetInventoryModel()->CollisionChack(Mouse::Get()->GetPosition()) && Mouse::Get()->Down(0))
				{
					// ù ������ ���� �������� ��ȣ�� ��ġ�� ������.
					MouseBag.first.first = i;
					MouseBag.first.second = false;

					// ������ �� ���� ���ڿ�  �� ���� �����͸� ������.
					MouseBag.second = Under_inventorymap[i];

					// �����־��� �������� �����ʹ� null�� �ʱ�ȭ����.
					Under_inventorymap[i].count = 0;
					Under_inventorymap[i].block = nullptr;
					isDrag = true;

					return; // ���콺�� ������ ��
				}
			}

		}

		FOR(27)
		{
			if (inventorymap[i].block != nullptr) {
				// ���콺�� ������
				if (inventorymap[i].block->GetInventoryModel()->CollisionChack(Mouse::Get()->GetPosition()) && Mouse::Get()->Down(0))
				{
					// ù ������ ���� �������� ��ȣ�� ��ġ�� ������.
					MouseBag.first.first = i;
					MouseBag.first.second = true;

					// ������ �� ���� ���ڿ�  �� ���� �����͸� ������.
					MouseBag.second = inventorymap[i];

					// �����־��� �������� �����ʹ� null�� �ʱ�ȭ����.

					inventorymap[i].count = 0;
					inventorymap[i].block = nullptr;
					isDrag = true;

					return; // ���콺�� ������ ��
				}
			}
		}
	}


}

void MineCraftUI::Drop()
{
	// Drop
	if (!Mouse::Get()->Press(0) && !Mouse::Get()->Down(0) && isDrag) // �ȴ����� ������. ����������. �巡�����϶��� ����.
	{
		// �ϴ� �巡�״� ���̴ϱ� ���̶�� �˷���
		isDrag = false;
		if (MouseBag.second.block != nullptr) // ������ �� bag�� nullptr�� �ƴҰ��.
		{
			Vector3 nowpos = Mouse::Get()->GetPosition();

			// �κ��丮 â�� �����ִ��� ���� üũ �κ��丮 â ������ Ȯ��.
			// �κ��丮â �ȿ� �������,
			if (Inventory->CollisionChack(nowpos))
			{
				//�˻�
				float minfloat = FLT_MAX;
				UINT insertpos;
				bool insert;

				FOR(27)
				{
					float len = (inventorymap[i].pos - nowpos).Length();
					if (minfloat > len && 
						(inventorymap[i].block == nullptr || inventorymap[i].block->GetBlockData().name == MouseBag.second.block->GetBlockData().name)) {
						minfloat = len;
						insertpos = i;
						insert = true;
					}
				}

				FOR(9)
				{
					float len = (Under_inventorymap[i].pos2 - nowpos).Length();
					if (minfloat > len && 
					(Under_inventorymap[i].block == nullptr || Under_inventorymap[i].block->GetBlockData().name == MouseBag.second.block->GetBlockData().name)) {
						minfloat = len;
						insertpos = i;
						insert = false;
					}
				}

				// ��ӽ� ���� ����� ĭ�� �־���.
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
						(Under_inventorymap[i].block == nullptr ||
							Under_inventorymap[i].block->GetBlockData().name == MouseBag.second.block->GetBlockData().name)) {
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

		// �� �������� ���� ���ߴµ� ����ߴٸ� ���� ��ġ�� ��ȯ��.
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

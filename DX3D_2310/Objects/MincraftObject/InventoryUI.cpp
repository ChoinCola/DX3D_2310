#include "Framework.h"
#include "InventoryUI.h"

InventoryUI::InventoryUI(wstring textureFile)
	: Quad(textureFile)
{
	SetTag("InventoryPanal");
	Load();
}

InventoryUI::~InventoryUI()
{
	Save();
}

void InventoryUI::Update()
{
	if (Mouse::Get()->Down(0) && CollisionChack(Mouse::Get()->GetPosition()))
	{
		isDrag = true;
		offset = GetGlobalPosition() - Mouse::Get()->GetPosition();
	}

	if (isDrag && Mouse::Get()->Press(0))
	{
		SetLocalPosition(Mouse::Get()->GetPosition() + offset);
	}

	if (Mouse::Get()->Up(0))
		isDrag - false;

	UpdateWorld();
	InventoryposUpdate();
}

void InventoryUI::PosRender()
{
	Quad::Render();
	InventoryRender();
}

void InventoryUI::InventoryRender()
{
	FOR(9)
		Under_inventorymap[i]->Render();

	FOR(27)
		inventorymap[i]->Render();
}

void InventoryUI::InventoryposUpdate()
{
	FOR(9)
		Under_inventorymap[i]->Update();

	FOR(27)
		inventorymap[i]->Update();
}

void InventoryUI::Drag()
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

void InventoryUI::Drop()
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
			if (CollisionChack(nowpos))
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
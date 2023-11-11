#include "Framework.h"
#include "InventoryUI.h"

InventoryUI::InventoryUI(wstring textureFile)
	: Quad(textureFile)
{
	FOR(9)
	{
		Under_inventorymap[i] = new InvenBlock(inventoryBaseDown + Vector3(i * 16, 0, 0), this);
	}

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			Under_inventorymap[i * 9 + j] = new InvenBlock(inventoryBase + Vector3(j * 16, i * 16, 0), this);
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
	// Drag 중이 아닐때만 동작함.
	// 밑 칸은 false, 윗칸은 true
	if (!isDrag)
	{
		FOR(9)
		{
			if (Under_inventorymap[i] != nullptr) {
				// 마우스가 집으면
				if (Under_inventorymap[i]->)
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

void InventoryUI::Drop()
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
			if (CollisionChack(nowpos))
			{
				//검사
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

		// 그 무엇에도 넣지 못했는데 드롭했다면 원래 위치로 귀환함.
		if (MouseBag.first.second)
			inventorymap[MouseBag.first.first] = MouseBag.second;
		else
			Under_inventorymap[MouseBag.first.first] = MouseBag.second;

		MouseBag.second.clear();
	}

}
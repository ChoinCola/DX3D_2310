#pragma once


class InventoryUI : public Quad
{
public:
	InventoryUI(wstring textureFile);
	~InventoryUI();

	void Update();
	void PostRender();

	void insertblock(Block* block);

	InvenBlock* GetUnderinventory(UINT number) { return Under_inventorymap[number]; }

	void SetDrag() { isDrag = !isDrag; }

private:
	void InventoryRender();
	void InventoryposUpdate();

	void Under_inventoryViewUpdate();
	void Under_inventoryViewRender();

	void Update_MouseBag();
	void PostRender_MouseBag();

	void Drop();

private:
	Vector3 dragoffset;

	unordered_map<UINT, InvenBlock*> inventorymap;
	Vector3 inventoryBase = { -18 * 4, -18 * 2.5, 0.0f };
	Vector3 inventoryBaseDown = { -18 * 4, -18 * 3.5 - 4, 0.0f };


	unordered_map<UINT, InvenBlock*> Under_inventorymap;
	vector<Quad*> Under_inventoryView;

	Vector3 Under_inventorymapBaseDown = { CENTER_X - 160, 20, 0.0f };

	pair<InvenBlock*, pair<UINT,Block*>> MouseBag;
	Quad* MouseObject;

	bool isDrag = false;
};
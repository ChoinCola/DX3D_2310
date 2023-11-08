#pragma once

struct InvenBlock
{
	void clear() 
	{
		count = 0;
		block = nullptr;
		pos.Zero();
		pos2.Zero();
	}

	UINT count;
	Block* block;
	Vector3 pos;
	Vector3 pos2;
};

class MineCraftUI
{
public:
	MineCraftUI();
	~MineCraftUI();

	void Update();
	void PostRender();
	void GUIRender();
	void Mining();
	void Build();
	
private:
	void Under_UI();
	void ViewInventory();
	void InventoryRender();
	void Drag();
	void Drop();
	void MouseBagUpdate();
	void MouseBagRender();
	bool InsertInventory(Block* input);

private:

	UINT nowselect = 0;
	UINT Maxselect = 8;
	Vector3 SelectBasePosition;

	Quad* Inventory;
	Quad* UnderInventory;
	Quad* Select;

	bool SelectType = true;

	unordered_map<UINT, InvenBlock> inventorymap;
	Vector3 inventoryBase = { 496, 342, 0.0f };
	Vector3 inventoryBaseDown = { 496, 226, 0.0f };

	unordered_map<UINT, InvenBlock> Under_inventorymap;
	Vector3 Under_inventorymapBaseDown = { CENTER_X - 160, 20, 0.0f };


	vector<Quad*> UIVector;

	pair<pair<UINT, bool>, InvenBlock> MouseBag;
	bool isDrag = false;
};
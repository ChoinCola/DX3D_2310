#pragma once

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

	void MouseBagUpdate();
	void MouseBagRender();
	bool InsertInventory(Block* input);

private:

	UINT nowselect = 0;
	UINT Maxselect = 8;
	Vector3 SelectBasePosition;

	InventoryUI* inventory;

	Quad* UnderInventory;
	Quad* Select;

	bool SelectType = true;

	vector<Quad*> UIVector;


	bool isDrag = false;
};
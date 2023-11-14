#pragma once

class ShopUI : public Quad
{
public:
	ShopUI(wstring textureFile);
	~ShopUI();

	void Update();
	void PostRender();
	void GUIRender();
	void insertblock(Block* block);

	void SetDrag() { isDrag = !isDrag; }

	void IsOpenUI(bool input);

private:
	void InventoryRender();
	void InventoryposUpdate();

	void Under_inventoryViewUpdate();
	void Under_inventoryViewRender();

private:
	Vector3 dragoffset;

	unordered_map<UINT, InvenBlock*> Itemmap;
	Vector3 ItemmapBase = { -18 * 4, -18 * 3 - 4, 0.0f };

	unordered_map<UINT, InvenBlock*> Buymap;
	Vector3 BuymapBase = { 18 * 2, -18 * 3 - 4, 0.0f };

	Transform* moneypos;

	bool isDrag = false;
	int Buymapmoney = 0;
};
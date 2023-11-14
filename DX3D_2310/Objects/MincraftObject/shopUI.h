#pragma once

class ShopUI : public Quad
{
public:
	ShopUI(wstring textureFile);
	~ShopUI();

	void Update();
	void PostRender();
	void GUIRender();
	void insertItemmap(Block* block);
	void insertItemmap(InvenBlock* block);
	void insertBuymap(Block* block, UINT count = 1, bool property = true);
	void Trade();
	void SetDrag() { isDrag = !isDrag; }

	void IsOpenUI(bool input);
	void Rotate_TradeOutline();
private:
	void InventoryRender();
	void InventoryposUpdate();

	void TradeButtonRender();
	void TradeButtonUpdate();

	InvenBlock* GetInvenBlockdataInitemmap(int num);

private:
	Vector3 dragoffset;

	unordered_map<UINT, InvenBlock*> Itemmap;
	Vector3 ItemmapBase = { -18 * 4, -18 * 3 - 4, 0.0f };

	unordered_map<UINT, InvenBlock*> Buymap;
	Vector3 BuymapBase = { 18 * 2, -18 * 3 - 4, 0.0f };

	Transform* moneypos;

	Button* TradeButton;
	Quad* TradeOutline;

	bool isDrag = false;
	int Buymapmoney = 0;
};
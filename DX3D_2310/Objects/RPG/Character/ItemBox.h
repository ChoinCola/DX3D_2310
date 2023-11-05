#pragma once

static UINT ItemBoxnum = 0;
class ItemBox
{
public:
	ItemBox(Float2 Boxsize = { 300, 300 }, string name = "null");
	~ItemBox();

	void RenderUI();
	void Update();

	bool CloaseCollisionCheck();
	bool& GetIsCloase() { return IsCloase; }
private:
	void DeleteItem();
	void ChackItem();
	void UseItem();

	void Load();
	void Save();
private:

	string teg;
	UINT number;

	list<Item*> items;
	Quad* itemUI;

	Quad* CloaseButton;

	Quad* UpBar;

	Vector3 ClaoseButton;
	Vector3 OpenButton;
	Vector3 DeleteButton;

	bool IsCloase = false;
};
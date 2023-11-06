#pragma once
class Item : public BoxCollider
{
public:
	Item(string Id = "null", UINT* numberpointer = nullptr);
	~Item();

	void InsertItem();
	void OutItem();
	void Update();
	void Render();
	void GUIRender();

	Model* GetModel() { return Object; }
	virtual void ItemRender() = 0;
	virtual void UseItem() = 0;
	
	const string GetItemId() { return ID; }

protected:
	void Load();
	void Save();

protected:
	string ID = {};
	UINT* Objectnumber;

	Model* Object = nullptr;
	Quad* itemUI = nullptr;

	UINT ItemSizeWidth	= 0;
	UINT ItemSizeHeight	= 0;

	bool IsRender = true;
};
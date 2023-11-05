#pragma once
static UINT TableCount = 0;

class Table : public Item
{
public:
	Table() : Item("Table", &count) {
		count = ChairCount;
		Object = new Model("Table");
		Object->SetTag("TableModel" + count);
		SetTag("TableCollision" + count);

		itemUI = new Quad();
		itemUI->GetMaterial()->SetDiffuseMap(L"Textures/Colors/White.png");
		Object->Load();
		Load();

		ItemSizeWidth	= 3;
		ItemSizeHeight	= 3;

		ChairCount++;
	};
	~Table();

	// Item을(를) 통해 상속됨
	void ItemRender() override;
	void UseItem() override;

private:
	UINT count = 0;
};
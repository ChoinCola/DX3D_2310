#pragma once
static UINT BoxCount = 0;

class Box : public Item
{
public:
	Box() : Item("Box", &count) {
		count = ChairCount;
		Object = new Model("Table");
		Object->SetTag("BoxModel" + count);
		SetTag("BoxCollision" + count);

		itemUI = new Quad();
		itemUI->GetMaterial()->SetDiffuseMap(L"Textures/Colors/White.png");
		Object->Load();
		Load();

		ItemSizeWidth = 3;
		ItemSizeHeight = 3;

		ChairCount++;
	};
	~Box();

	// Item��(��) ���� ��ӵ�
	void ItemRender() override;
	void UseItem() override;

private:
	UINT count = 0;
};
#pragma once

static UINT ChairCount = 0;

class Chair : public Item
{
public:
	Chair() : Item("Chair", &count) {
		count = ChairCount;
		Object = new Model("Chair");
		Object->SetTag("ChairModel" + count);
		SetTag("ChairCollision" + count);
		itemUI = new Quad();
		itemUI->GetMaterial()->SetDiffuseMap(L"Textures/Colors/White.png");
		Object->Load();
		Load();

		ItemSizeWidth = 2;
		ItemSizeHeight = 2;
		ChairCount++;
	};
	~Chair();

	// Item을(를) 통해 상속됨
	void ItemRender() override;
	void UseItem() override;

private:
	UINT count = 0;
	
	

};
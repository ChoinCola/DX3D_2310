#pragma once

class InvenBlock : public Button
{
public:
	InvenBlock(Vector3 pos, Transform* Parent) : Button(Float2(32, 32)) {};
	~InvenBlock() = default;

	void clear()
	{
		count = 0;
		SAFE_DELETE(block);
	}
	void Update() { UpdateWorld(); }

	void InsertBlock(Block* block) { 
		if (block != nullptr && (block->GetBlockData().name == this->block->GetBlockData().name)) {
			delete block;
			count++;
			return;
		}

		block->GetInventoryModel()->SetLocalPosition(GetLocalPosition());
		this->block = block; 
	}

	Block* GetBlock() 
	{
		if (count == 0) {
			clear();
			return block;
		}
		else count--;

		Block* result = nullptr;
		if(block != nullptr)
			result = new Block(block->GetBlockData());

		return result;
	}

	void PostRneder()
	{
		if(block != nullptr)
			block->GetInventoryModel()->Render();
	}

private:
	UINT count = 0;
	Block* block = nullptr;
};

class InventoryUI : public Quad
{
public:
	InventoryUI(wstring textureFile);
	~InventoryUI();

	void Update();
	void PosRender();

	InvenBlock& GetUnderinventory(UINT number) { return *Under_inventorymap[number]; }

private:
	void InventoryRender();
	void InventoryposUpdate();

	void Drag();
	void Drop();


private:
	Vector3 offset;

	unordered_map<UINT, InvenBlock*> inventorymap;
	Vector3 inventoryBase = { 496, 342, 0.0f };
	Vector3 inventoryBaseDown = { 496, 226, 0.0f };


	unordered_map<UINT, InvenBlock*> Under_inventorymap;
	Vector3 Under_inventorymapBaseDown = { CENTER_X - 160, 20, 0.0f };

	pair<pair<UINT, bool>, InvenBlock*> MouseBag;
	bool isDrag = false;
};
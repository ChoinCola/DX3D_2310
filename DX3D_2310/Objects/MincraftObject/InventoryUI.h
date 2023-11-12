#pragma once

class InvenBlock : public Button
{
public:
	InvenBlock(Vector3 pos, Transform* Parent = nullptr) : Button(Float2(16, 16)), mainPos(pos){
		GetMaterial()->SetDiffuseMap(L"Textures/Colors/Blue.png");
		SetLocalPosition(pos);
		//IsRender();
		if(Parent != nullptr)
			SetParent(Parent);
	};

	~InvenBlock() = default;

	void clear()
	{
		count = 0;
		block = nullptr;
	}

	void Update() { 
		__super::Update();
		if (block != nullptr)
			block->Update();
		UpdateWorld(); 
	}

	bool InsertBlock(Block* block) { 
		if (block != nullptr && this->block == nullptr) {
			block->GetInventoryModel()->SetParent(this);
			block->GetInventoryModel()->SetLocalScale(Vector3(0.5, 0.5, 1));
			this->block = block;
			this->count = 1;
			return true;
		}

		if (block != nullptr && (block->GetBlockData().name == this->block->GetBlockData().name)) {
			delete block;
			count++;
			return true;;
		}
		return false;
	}

	void InsertBlock(InvenBlock* block) {
		if (block != nullptr && ChackBlock(block)) {
			count += block->GetCount();
			delete block;
			return;
		}

		if(block != nullptr)
			block->SetLocalPosition(mainPos);

		return;
	}

	Block* PopBlock() 
	{
		if (count == 1) {
			Block* result = block;
			clear();
			return result;
		}
		else count--;

		Block* result = nullptr;
		if(block != nullptr)
			result = new Block(block->GetBlockData());

		return result;
	}

	bool ChackBlock(InvenBlock* block)
	{
		if (block->GetBlock()->GetBlockData().name == GetBlock()->GetBlockData().name)
			return true;
		return false;
	}

	void PostRender()
	{
		if (block != nullptr)
		{
			if (block->GetInventoryModel()->GetRender() == false)
				block->GetInventoryModel()->IsRender();

			string c = to_string(count);
			Vector3 pos = block->GetInventoryModel()->GetGlobalPosition();

			Font::Get()->RenderText(c,Float2(pos.x + 18, pos.y - 8));

			
			block->InventoryRender();
		}
	}

	void PopMouse(void* mouseBag)
	{
		pair<InvenBlock*, pair<UINT, Block*>>* input = static_cast<pair<InvenBlock*, pair<UINT, Block*>>*>(mouseBag);

		if (input->second.second == nullptr && input->second.first <= 0)
		{
			input->first = this;

			input->second.first = count;
			count = 0;

			input->second.second = block;
			block = nullptr;
		}
	}

	void InsertMouse(void* mouseBag)
	{
		pair<InvenBlock*, pair<UINT, Block*>>* input = static_cast<pair<InvenBlock*, pair<UINT, Block*>>*>(mouseBag);

		if (input->second.second == nullptr) return;

		if (block == nullptr || block->GetBlockData().modelname == input->second.second->GetBlockData().modelname) {
			SAFE_DELETE(block);
			count += input->second.first;

			input->first = nullptr;
			input->second.first = 0;
			input->second.second = nullptr;
			return;
		}

		input->first->SetBlock(input->second.second);
		input->first->GetBlock()->SetLocalPosition(Vector3(0, 0, 0));
		input->first->SetCount(input->second.first);
		input->second.first = 0;
		input->second.second = nullptr;
	}

	const UINT& GetBlockCount() { return count; }
	Block* GetBlock() { return block; }
	UINT GetCount() { return count; }

	void SetBlock(Block* input) { block = input; }
	void SetCount(UINT input) { count = input; }

private:
	UINT count = 0;
	Vector3 mainPos;
	Block* block = nullptr;
};

class InventoryUI : public Quad
{
public:
	InventoryUI(wstring textureFile);
	~InventoryUI();

	void Update();
	void PostRender();

	void insertblock(Block* block);

	InvenBlock* GetUnderinventory(UINT number) { return Under_inventorymap[number]; }

	void SetDrag() { isDrag = !isDrag; }

private:
	void InventoryRender();
	void InventoryposUpdate();

	void Under_inventoryViewUpdate();
	void Under_inventoryViewRender();

	void Update_MouseBag();
	void PostRender_MouseBag();


	void Drop();


private:
	Vector3 offset;

	unordered_map<UINT, InvenBlock*> inventorymap;
	Vector3 inventoryBase = { -18 * 4, -18 * 2.5, 0.0f };
	Vector3 inventoryBaseDown = { -18 * 4, -18 * 3.5 - 4, 0.0f };


	unordered_map<UINT, InvenBlock*> Under_inventorymap;
	vector<Quad*> Under_inventoryView;

	Vector3 Under_inventorymapBaseDown = { CENTER_X - 160, 20, 0.0f };

	pair<InvenBlock*, pair<UINT,Block*>> MouseBag;

	bool isDrag = false;
};
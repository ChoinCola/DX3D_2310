#pragma once
class InvenBlock : public Button
{
public:
	InvenBlock(Vector3 pos, Transform* Parent = nullptr) : Button(Float2(16, 16)), mainPos(pos) {
		GetMaterial()->SetDiffuseMap(L"Textures/Colors/Blue.png");
		SetLocalPosition(pos);

		if (Parent != nullptr)
			SetParent(Parent);
	};

	~InvenBlock() = default;

	void clear()
	{
		count = 0;
		block = nullptr;

		if (GetRender() == true)
			IsRender();
	}

	void Update() {
		__super::Update();
		if (block != nullptr)
			block->Update();
		UpdateWorld();
	}

	bool InsertBlock(Block* block, UINT count = 1) {
		if (block != nullptr && this->block == nullptr) {
			wstring name = ToWString(block->GetBlockData().modelname) + L".png";
			GetMaterial()->SetDiffuseMap(L"Textures/UI/Blocks/" + name);

			if (GetRender() == false)
				IsRender();

			this->block = block;
			this->count = count;
			return true;
		}

		if (block != nullptr && (block->GetBlockData().name == this->block->GetBlockData().name)) {
			delete block;
			this->count += count;
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

		if (block != nullptr)
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
		if (block != nullptr)
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
			string c = to_string(count);
			Vector3 pos = GetGlobalPosition();

			Font::Get()->RenderText(c, Float2(pos.x + 18, pos.y - 8));
			Render();
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
			if (InsertBlock(input->second.second, input->second.first))
			{
				input->first = nullptr;
				input->second.first = 0;
				input->second.second = nullptr;
				return;
			}
		}

		input->first->SetBlock(input->second.second);
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

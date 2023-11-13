#pragma once

class MouseBag : public Singleton<MouseBag>
{
private:

	friend class Singleton;

	MouseBag()
	{
		mouseObject = new Quad(L"Textures/UI/Blocks/block0.png");
	};
	~MouseBag()
	{
		SAFE_DELETE(mouseObject);
		SAFE_DELETE(Repos);
		SAFE_DELETE(block);
	};

public:
	void Render();
	void Update();

	InvenBlock*& GetBasePos() { return Repos; }
	UINT& GetCount() { return count; }
	Block*& GetBlock() { return block; }
	void SetBlock(Block* input) { block = input; }
	void SetHasPlayer(bool input) { HasPlayer = input; }
	bool GetHasPlayer() { return HasPlayer; }

private:
	InvenBlock* Repos = nullptr;
	Quad* mouseObject = nullptr;
	Block* block = nullptr;

	UINT count = 0;
	bool HasPlayer = true;
};
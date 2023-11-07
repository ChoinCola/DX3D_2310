#pragma once

class BlockManager : public Singleton<BlockManager>
{
private:
	friend class Singleton;

	BlockManager();
	~BlockManager();

public:
	void Update();
	void Redner();
	void GUIRender();

	void InsertBlocks(Ray mouseray);
	void DeleteBlocks(Ray mouseray);

	void CreateBlocks(UINT x, UINT y, UINT z);

	const list<Block*>& GetBlocks() { return blocks; }

private:
	list<Block*> blocks;
};
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

	void CreateBlocks(UINT x, UINT y, UINT z);

private:
	list<Block*> blocks;


};
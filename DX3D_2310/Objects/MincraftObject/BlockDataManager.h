#pragma once

struct BlockData
{
	int key;
	string name;
	string modelname;
	string uiimage;
	int cash;
	string itemtype;
	int damage;
};

class BlockDataManager : public Singleton<BlockDataManager>
{
private:
	friend class Singleton;
	BlockDataManager();
	~BlockDataManager();

public:
	void LoadTable();

	BlockData GetBlockData(int key) { return blockDatas[key]; }
	BlockData GetObjectDatas(int key) { return ObjectDatas[key]; }

	int GetBlockSize() { return blockDatas.size(); }

private:
	map<int, BlockData> blockDatas;
	map<int, BlockData> ObjectDatas;

};
#pragma once

class BlockManager : public Singleton<BlockManager>
{
private:
	const float PUSH_SPEED = 100.0f;

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

	void CollisionBlockToPlayer(SphereCollider& pos);
	float GetHeight(const Vector3& pos);
	Block* DeleteBlock(const Ray& ray);
	bool AddBlock(Block* block);

private:
	list<Block*> blocks;
};
#pragma once

class Block : public BoxCollider
{
public:
	Block(BlockData data);
	~Block();

	void Update();
	void Render();
	void InventoryRender();
	void GUIRender();
	Quad* GetInventoryModel() { return Inventorymodel; }
	BlockData GetBlockData() { return data; }
	Vector3 GetDirection(Vector3 point);

	
private:
	BlockData data;
	Model* model;

	Quad* Inventorymodel;
};
#pragma once

class Block : public BoxCollider
{
public:
	Block(BlockData data);
	~Block();

	void Update();
	void Render();
	void InventoryRender();
	void PostRender();
	void GUIRender();

	Quad* GetInventoryModel() { return Inventorymodel; }
	BlockData GetBlockData() { return data; }
	Vector3 GetDirection(Vector3 point);

private:
	Button* Inventorymodel;

	BlockData data;
	Model* model;
};
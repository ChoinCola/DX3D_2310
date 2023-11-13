#pragma once

class Block : public BoxCollider
{
public:
	Block(BlockData data);
	~Block();

	void Update();
	void Render();
	void PostRender();
	void GUIRender();

	BlockData GetBlockData() { return data; }
	Vector3 GetDirection(Vector3 point);

private:
	BlockData data;
	Model* model;
};
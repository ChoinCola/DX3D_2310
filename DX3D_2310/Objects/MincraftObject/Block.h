#pragma once

class Block : public BoxCollider
{
public:
	Block(string name);
	~Block();

	void Update();
	void Render();
	void InventoryRender();
	void GUIRender();
	Quad* GetInventoryModel() { return Inventorymodel; }
	string Getname() { return name; }
	Vector3 GetDirection(Vector3 point);

	
private:
	string name;

	Model* model;

	Quad* Inventorymodel;
};
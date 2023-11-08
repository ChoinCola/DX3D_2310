#pragma once

class Block : public BoxCollider
{
public:
	Block(string name);
	~Block();

	void Update();
	void Render();
	void GUIRender();

	Vector3 GetDirection(Vector3 point);

private:

	Model* model;
};
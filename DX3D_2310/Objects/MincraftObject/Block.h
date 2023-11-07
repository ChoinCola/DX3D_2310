#pragma once

class Block : public BoxCollider
{
public:
	Block(string name);
	~Block();

	void Update();
	void Render();
	void GUIRender();
private:
	Model* model;

};
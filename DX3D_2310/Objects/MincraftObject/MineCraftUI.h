#pragma once

class MineCraftUI
{
public:
	MineCraftUI();
	~MineCraftUI();

	void Update();
	void PostRender();

	void Mining();
	void Build();

private:
	Quad* cursor;

	list<Block*> blocks;
};
#pragma once

class Map 
{
	/*
	xz���� ��������, �ش� terrain�� player�� �ش�������� size�� �ΰ� ������� ���������, player�� �պκп� ��ġ��
	terrain���� ��ġ�� �̵���.
	*/

public:
	Map(Collider* player, Float2 size);
	~Map();

	void Update();
	void Render();
	void GUIRender();
	void SetScail();
private:
	void MakeTerrain();
	void MoveTerrain();

private:
	vector<Terrain*> map;
	Trees* trees;
	Collider* player;
	Float2 defaltsize;
};
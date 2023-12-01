#pragma once

class Map 
{
	/*
	xz축을 기준으로, 해당 terrain이 player와 해당방향으로 size의 두개 사이즈로 벌어질경우, player의 앞부분에 위치한
	terrain으로 위치를 이동함.
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
	Collider* player;
	Float2 defaltsize;
};
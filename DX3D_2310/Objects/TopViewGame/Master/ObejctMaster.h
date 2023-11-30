#pragma once

class ObjectMaster : public Singleton<Trees>
{
private:
	const UINT COUNT = 100;

private:
	friend class Singleton;


	ObjectMaster();
	~ObjectMaster();

	void Update();
	void Render();
	void PostRender();
	void RenderTerraintoTree(pair<float, float> nowpos, Float2 size);

private:
	void ChackRaytoCAM();
	void SetColor();
	void MoveAnimation();
	void MakeTree(pair<float, float> nowpos, Float2 size);
	void MakeGrass(pair<float, float> nowpos, Float2 size);
	void RenderObejcts(pair<float, float> nowpos);

private:
	map<pair<float, float>, vector<Transform>> treesmap;
	map<pair<float, float>, vector<Transform>> grasssmap;

	Tree* tree = nullptr;
	Grass* grass = nullptr;
};
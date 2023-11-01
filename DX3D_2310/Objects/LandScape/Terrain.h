#pragma once

class Terrain : public GameObject
{
private:
	typedef VertexUVNormal VertexType;
public:
	Terrain();
	Terrain(wstring HigntMap);
	Terrain(const wstring hightmap, const float hight = 1, bool tile = false, bool flip = false);

	~Terrain();

	void Render();
	void GUIRender();
	Vector3 GetOnGrondPosition(const Vector3 ObjectPos, const Vector3 correction = {0, 0, 0});
	int GetWidth() { return width; }
	int GetHeight() { return height; }

private:
	void MakeMesh(bool tile = false, bool flip = false);
	void MakeNormal();

private:
	UINT width = 10, height = 10;
	Mesh<VertexType>* mesh;
	Mesh<VertexColor>* normalline;
	RasterizerState* RSset;
	Texture* heightMap;
	int x = 0, z = 0;
	double y = 0;
	double D = 0;
	float hight = 1;


	Vector3 v;
	bool IsPrintNormalLine = false;
	bool IsMapLine = false;
	bool ChackOne = false;
};
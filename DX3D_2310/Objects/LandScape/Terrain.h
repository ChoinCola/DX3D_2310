#pragma once

class Terrain : public GameObject
{
private:
	typedef VertexUVNormal VertexType;
public:
	Terrain();
	~Terrain();

	void Render();
	void GUIRender();
	Vector3 GetOnGrondPosition(const Vector3 ObjectPos);
	int GetWidth() { return width; }
	int GetHeight() { return height; }

private:
	void MakeMesh();
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

	Vector3 v;
	bool IsPrintNormalLine = false;
	bool IsMapLine = false;
	bool ChackOne = false;
};
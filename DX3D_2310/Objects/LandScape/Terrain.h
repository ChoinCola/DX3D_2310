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
private:
	void MakeMesh();
	void MakeNormal();

private:
	UINT width = 10, height = 10;
	Mesh<VertexType>* mesh;
	Mesh<VertexColor>* normalline;
	RasterizerState* RSset;
	Texture* heightMap;

	bool IsPrintNormalLine = false;
	bool IsMapLine = false;
	bool ChackOne = false;
};
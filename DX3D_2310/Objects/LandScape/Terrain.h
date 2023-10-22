#pragma once

class Terrain : public GameObject
{
private:
	typedef VertexUV VertexType;
public:
	Terrain();
	~Terrain();

	void Render();
	void GUIRender();
private:
	void MakeMesh();
	void MakeNormal();
	Vector3 MakeNormalline(Vector3 x, Vector3 y, Vector3 z);
private:
	UINT width = 10, height = 10;
	Mesh<VertexUVNormal>* mesh;
	Mesh<VertexColor>* normalline;
	RasterizerState* RSset;
	Texture* heightMap;

	bool IsPrintNormalLine = false;
	bool IsMapLine = false;
	bool ChackOne = false;
};
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
	Vector3 GetOnGrondPosition(Vector3 ObjectPos);
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
	float y = 0;
	float D = 0;

	Vector3 v;
	bool IsPrintNormalLine = false;
	bool IsMapLine = false;
	bool ChackOne = false;
};
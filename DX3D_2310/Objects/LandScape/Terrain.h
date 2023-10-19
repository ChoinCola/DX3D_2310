#pragma once

class Terrain : public GameObject
{
private:
	typedef VertexUV VertexType;
public:
	Terrain();
	~Terrain();

	void Render();

private:
	void MakeMesh();

private:
	UINT width = 10, height = 10;
	Mesh<VertexType>* mesh;

	Texture* heightMap;
};
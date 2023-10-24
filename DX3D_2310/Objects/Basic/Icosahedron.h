#pragma once

class Icosahedron : public GameObject
{
private:
	typedef VertexUVNormalTangent VertexType;
public:
	Icosahedron(float size = 1, UINT dividecount = 0);
	~Icosahedron();

	void Update();
	void Render();
	void GUIRender();
private:
	void BackUpMesh();
	void MakeMesh(float size, UINT dividecount);
	void Subdivide();
	void MakeNormal();
	VertexUVNormalTangent MidPoint(const VertexType& v0, const VertexType& v1);
private:

	Mesh<VertexType>* mesh;
	Mesh<VertexType>* BackUpmesh;
	RasterizerState* RSset;

	int size;
	bool Setline = 0;
	UINT dividecountold = 0;
	UINT dividecountnow = 0;
};
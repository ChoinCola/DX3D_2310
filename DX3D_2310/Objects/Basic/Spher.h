#pragma once

class Spher : public GameObject
{
private:
	typedef VertexUVNormal VertexType;
public:
	Spher(float size = 1, UINT dividecount = 0);
	~Spher();

	void Update();
	void Render();
	void GUIRender();
private:
	void BackUpMesh();
	void MakeMesh(float size, UINT dividecount);
	void Subdivide();
	void MakeNormal();
	VertexUVNormal MidPoint(const VertexType& v0, const VertexType& v1);
private:

	Mesh<VertexType>* mesh;
	Mesh<VertexType>* BackUpmesh;
	RasterizerState* RSset;

	int size;
	bool Setline = 0;
	UINT dividecountold = 0;
	UINT dividecountnow = 0;
};
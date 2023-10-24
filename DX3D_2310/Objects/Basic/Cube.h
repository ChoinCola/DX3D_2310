#pragma once

class Cube : public GameObject
{
public:
	typedef VertexUVNormalTangent VertexType;
	Cube(Float3 size = { 1, 1, 1}, Vector3 pos = {0, 0, 0});
	~Cube();

	void Render();

	Material* GetMetrial() { return material; }
private:
	void MakeMesh();
	void MakeTangent();
	void MakeNormal();
private:
	Mesh<VertexType>* mesh;
	Float3 size;
};
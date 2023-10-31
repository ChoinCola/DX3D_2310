#pragma once
class Quad : public GameObject
{
public:
	typedef VertexUVNormalTangent VertexType;

	Quad(Float2 size = { 1, 1 });
	Quad(wstring texturefile);
	~Quad();

	void Render();
private:
	void MakeMesh();
private:
	Mesh<VertexType>* mesh;

	Float2 size;
};
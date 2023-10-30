#pragma once
class Quad : public GameObject
{
public:
	typedef VertexUV VertexType;

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
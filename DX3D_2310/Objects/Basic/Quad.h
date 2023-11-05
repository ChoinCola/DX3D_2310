#pragma once
class Quad : public GameObject
{
public:
	typedef VertexUVNormalTangent VertexType;

	Quad(Float2 size = { 100, 1000 });
	Quad(wstring texturefile);
	~Quad();

	void Render();

	bool CollisionChack(Vector3 inputpos);

private:
	void MakeMesh();
private:
	Mesh<VertexType>* mesh;

	Float2 size;
};
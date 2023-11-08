#pragma once
class Quad : public GameObject
{
public:
	typedef VertexUVNormalTangent VertexType;

	Quad(Float2 size = { 100, 1000 });
	Quad(wstring texturefile);
	~Quad();

	void Render();

	void IsRender() { render = !render; }

	bool CollisionChack(Vector3 inputpos);

	const bool GetRender() const { return render; }
private:
	void MakeMesh();
private:

	bool render = true;
	Mesh<VertexType>* mesh;

	Float2 size;
};
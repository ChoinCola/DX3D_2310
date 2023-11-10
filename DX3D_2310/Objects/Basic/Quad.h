#pragma once
class Quad : public GameObject
{
public:
	typedef VertexUVNormalTangent VertexType;

	Quad(Float2 size = { 1, 1 });
	Quad(wstring texturefile);
	~Quad();

	void Render();

	Float2 GetSize()
	{
		return 
		{ GetGlobalScale().x * size.x, 
			GetGlobalScale().y * size.y };
	}

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
#pragma once

class Cube : public GameObject
{
public:
	Cube(Float3 size = { 1, 1, 1}, Vector3 pos = {0, 0, 0});
	~Cube();

	void Render();

	Material* GetMetrial() { return material; }

private:
	Mesh<VertexUV>* mesh;
};
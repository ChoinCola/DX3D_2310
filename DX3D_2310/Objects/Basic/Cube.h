#pragma once

class Cube : public Transform
{
public:
	Cube(Float3 size = { 1, 1, 1}, Vector3 pos = {0, 0, 0});
	~Cube();

	void Update();
	void Render();

	Material* GetMetrial() { return material; }

private:
	Material* material;
	Mesh<VertexUV>* mesh;

	MatrixBuffer* worldBuffer;
};
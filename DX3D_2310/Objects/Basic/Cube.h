#pragma once

class Cube : public Transform
{
public:
	Cube(Float3 size = { 1, 1, 1});
	~Cube();

	void Update();
	void Render();

private:
	Material* material;
	Mesh<VertexUV>* mesh;

	MatrixBuffer* worldBuffer;

	ID3D11ShaderResourceView* srv;
};
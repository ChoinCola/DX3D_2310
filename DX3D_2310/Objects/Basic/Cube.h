#pragma once

class Cube : public Transform
{
public:
	Cube(Float3 size = { 1, 1, 1});
	~Cube();

	void Update();
	void Render();

private:
	VertexShader* vertexShader;
	PixelShader* pixelShader;

	VertexBuffer* vertexBuffer;
	IndexBuffer* indexBuffer;

	MatrixBuffer* worldBuffer;

	vector<VertexColor> vertices;
	vector<UINT> indices;
};
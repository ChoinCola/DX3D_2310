#pragma once

class Cube
{
public:
	Cube(Float3 size = { 1, 1, 1});
	~Cube();

	void Update();
	void Render();

	void SetPos(float x, float y, float z) { pos = { x, y, z }; }
private:
	VertexShader* vertexShader;
	PixelShader* pixelShader;

	VertexBuffer* vertexBuffer;
	IndexBuffer* indexBuffer;

	MatrixBuffer* worldBuffer;

	vector<VertexColor> vertices;
	vector<UINT> indices;

	Float3 pos = {};
};
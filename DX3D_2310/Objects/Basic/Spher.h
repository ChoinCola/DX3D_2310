#pragma once

class Spher : public Transform
{
public:
	Spher(float size = 1, UINT dividecount = 0);
	~Spher();

	void Update();
	void Render();
private:
	void Subdivide();
	VertexColor MidPoint(const VertexColor& v0, const VertexColor& v1);
private:
	VertexShader* vertexShader;
	PixelShader* pixelShader;

	VertexBuffer* vertexBuffer;
	IndexBuffer* indexBuffer;

	MatrixBuffer* worldBuffer;

	vector<VertexColor> vertices;
	vector<UINT> indices;
};
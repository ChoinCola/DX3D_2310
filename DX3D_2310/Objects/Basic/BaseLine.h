#pragma once

class BaseLine : public Transform
{
public:
	BaseLine(float size = 1, int Maxlinesize = 1000);
	~BaseLine();

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
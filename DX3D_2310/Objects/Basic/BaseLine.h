#pragma once

class BaseLine : public Transform
{
public:
	BaseLine(float size = 1, int Maxlinesize = 1000);
	~BaseLine();

	void Update();
	void Render();

private:
	Material* material;
	Mesh<VertexColor>* mesh;
	MatrixBuffer* worldBuffer;
};
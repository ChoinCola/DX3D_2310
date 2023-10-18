#pragma once
class Ground : public Transform
{
public:
	Ground(UINT Size, UINT BlockCount);
	~Ground();

	void Update();
	void DivideUpdate(UINT inputNumber);
	void Render();

	Material* GetMetrial() { return material; }
private:
	VertexUV MidPoint(const VertexUV& v0, const VertexUV& v1);
	void Subdivide();

private:
	Material* material;
	Mesh<VertexUV>* mesh;

	Mesh<VertexUV>* Backupmesh;

	MatrixBuffer* worldBuffer;

	UINT nowDivideNumber = 0;
};
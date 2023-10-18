#pragma once
class Ground : public Transform
{
public:
	Ground(UINT MaxSize, UINT BlockSize);
	~Ground();

	void Update();
	void Render();

	Material* GetMetrial() { return material; }

private:
	Material* material;
	Mesh<VertexUV>* mesh;

	MatrixBuffer* worldBuffer;
};
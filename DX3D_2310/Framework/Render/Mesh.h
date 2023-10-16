#pragma once

template<typename T>
class Mesh
{
public:
	Mesh() = default;
	~Mesh();

	void Draw(D3D11_PRIMITIVE_TOPOLOGY type = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	void CreateMesh();

	vector<T>& GetVertices() { return vertices; }
	vector<UINT>& GetIndices() { return indices; }


private:
	VertexBuffer* vertexBuffer = nullptr;
	IndexBuffer* indexBuffer = nullptr;

	vector<T> vertices;
	vector<UINT> indices;
};

template<typename T>
inline Mesh<T>::~Mesh()
{
	delete vertexBuffer;

	if(indexBuffer != nullptr)
		delete indexBuffer;
}

template<typename T>
inline void Mesh<T>::Draw(D3D11_PRIMITIVE_TOPOLOGY type)
{
	vertexBuffer->Set(type);

	if (indexBuffer)
	{
		indexBuffer->Set();
		DC->DrawIndexed(indices.size(), 0, 0);	
}
	else
	{
		DC->Draw(vertices.size(), 0);
	}
}

template<typename T>
inline void Mesh<T>::CreateMesh()
{
	vertexBuffer = new VertexBuffer(vertices.data(),
		sizeof(T), vertices.size());

	if (indices.size() > 0)
	{
		indexBuffer = new IndexBuffer(indices.data(), indices.size());
	}
}

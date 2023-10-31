#include "Framework.h"

Quad::Quad(Float2 size) : size(size)
{
	tag = "Quad";

	mesh = new Mesh<VertexType>();
	MakeMesh();
	mesh->CreateMesh();
}

Quad::Quad(wstring texturefile)
{
	Texture* texture = Texture::Add(texturefile);
	material->SetDiffuseMap(texturefile);
	size = texture->GetSize();

	mesh = new Mesh<VertexType>();
	MakeMesh();
	mesh->CreateMesh();
}

Quad::~Quad()
{
	delete mesh;
}

void Quad::Render()
{
	SetRender();
	mesh->Draw();
}

void Quad::MakeMesh()
{
	Float2 hlafSize(size.x * 0.5f, size.y * 0.5f);

	vector<VertexType>& vertices = mesh->GetVertices();

	vertices.emplace_back(-hlafSize.x, -hlafSize.y, 0, 0, 1);
	vertices.emplace_back(-hlafSize.x, +hlafSize.y, 0, 0, 0);
	vertices.emplace_back(+hlafSize.x, -hlafSize.y, 0, 1, 1);
	vertices.emplace_back(+hlafSize.x, +hlafSize.y, 0, 1, 0);

	for (VertexType& vertex : vertices)
	{
		vertex.normal = { 0, 0, -1 };
		vertex.tangent = { 1, 0, 0 };

	}
	vector<UINT>& indices = mesh->GetIndices();

	indices =
	{
		0, 1, 2, 2, 1, 3,
		2, 1, 0, 3, 1, 2
	};

}

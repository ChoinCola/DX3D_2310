#include "Framework.h"

Quad::Quad(Float2 size) : size(size)
{
	tag = "Quad";

	mesh = new Mesh<VertexType>();
	MakeMesh();
	mesh->CreateMesh();
}

Quad::Quad(wstring texturefile)
	: GameObject(L"Basic/Texture.hlsl")
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
	if (!IsActive()) return;

	SetRender();
	mesh->Draw();

}

void Quad::RenderInstanced(UINT count)
{
	if (!IsActive()) return;

	SetRender();
	mesh->DrawInstanced(count);
}

bool Quad::CollisionChack(Vector3 inputpos)
{
	if (IsActive()) return false;
	Float2 halfsize(size.x * 0.5f * localScale.x, size.y * 0.5f * localScale.y);
	Vector3 pos = GetGlobalPosition();

	RECT quad = { pos.x - halfsize.x, pos.y + halfsize.y, pos.x + halfsize.x, pos.y - halfsize.y };

	if (inputpos.x >= quad.left && inputpos.x <= quad.right &&
		inputpos.y >= quad.bottom && inputpos.y <= quad.top)
		return true;

	return false;
}

void Quad::MakeMesh()
{
	Float2 halfSize(size.x * 0.5f, size.y * 0.5f);

	vector<VertexType>& vertices = mesh->GetVertices();

	vertices.emplace_back(-halfSize.x, -halfSize.y, 0, 0, 1);
	vertices.emplace_back(-halfSize.x, +halfSize.y, 0, 0, 0);
	vertices.emplace_back(+halfSize.x, -halfSize.y, 0, 1, 1);
	vertices.emplace_back(+halfSize.x, +halfSize.y, 0, 1, 0);

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
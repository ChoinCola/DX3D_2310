#include "Framework.h"

GridScene::GridScene()
{
	material = new Material(L"Basic/Grid.hlsl");

	mesh = new Mesh<VertexColor>();
	MakeMesh();
	mesh->CreateMesh();

	worldBuffer = new MatrixBuffer();
}

GridScene::~GridScene()
{
	delete material;
	delete mesh;
	delete worldBuffer;
}

void GridScene::Update()
{
}

void GridScene::PreRender()
{
}

void GridScene::Render()
{
	worldBuffer->SetVS(0);

	material->Set();
	mesh->Draw(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
}

void GridScene::PostRender()
{
}

void GridScene::GUIRender()
{
}

void GridScene::MakeMesh()
{
    vector<VertexColor>& vertices = mesh->GetVertices();

    //Axis X
    vertices.emplace_back(-MAX_SIZE, 0, 0, 1.0f, 0.5f, 0.5f);
    vertices.emplace_back(+MAX_SIZE, 0, 0, 1.0f, 0.5f, 0.5f);

    //Axis Y
    vertices.emplace_back(0, -MAX_SIZE, 0, 0.5f, 1.0f, 0.5f);
    vertices.emplace_back(0, +MAX_SIZE, 0, 0.5f, 1.0f, 0.5f);

    //Axis Z
    vertices.emplace_back(0, 0, -MAX_SIZE, 0.5f, 0.5f, 1.0f);
    vertices.emplace_back(0, 0, +MAX_SIZE, 0.5f, 0.5f, 1.0f);

    int halfSize = MAX_SIZE >> 1;

    for (int x = -halfSize; x <= halfSize; x++)
    {
        if (x == 0) continue;

        vertices.emplace_back(x, 0, -halfSize, 0.5f, 0.5f, 0.5f);
        vertices.emplace_back(x, 0, +halfSize, 0.5f, 0.5f, 0.5f);
    }

    for (int z = -halfSize; z <= halfSize; z++)
    {
        if (z == 0) continue;

        vertices.emplace_back(-halfSize, 0, z, 0.5f, 0.5f, 0.5f);
        vertices.emplace_back(+halfSize, 0, z, 0.5f, 0.5f, 0.5f);
    }
}

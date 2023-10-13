#include "Framework.h"

BaseLine::BaseLine(float size, int Maxlinesize)
{
    float lineSize = Maxlinesize * 0.5;
    material = new Material(L"VertexColorShader.hlsl", VERTEX_COLOR);
    mesh = new Mesh<VertexColor>();

    vector<VertexColor>& vertices = mesh->GetVertices();
    for (int i = 0; i <= Maxlinesize / size; i ++) {
        vertices.emplace_back(-lineSize + i * size, 0.0f, +lineSize,  0.25, 0.25, 0.25);
        vertices.emplace_back(-lineSize + i * size, 0.0f, -lineSize,  0.25, 0.25, 0.25);
        vertices.emplace_back(-lineSize, 0.0f, -lineSize + i * size, 0.25, 0.25, 0.25);
        vertices.emplace_back(+lineSize, 0.0f, -lineSize + i * size, 0.25, 0.25, 0.25);
        //01 23 45 67 89
    }

    // 마지막 Vertex의 위치값 또한 입력 +2
    vector<UINT>& indices = mesh->GetIndices();
    for (int i = 0; i <= Maxlinesize / size * 4 + 2; i+=2) {
        indices.emplace_back(i);
        indices.emplace_back(i + 1);
    }

    mesh->CreateMesh();

    material->SetShader(L"VertexColorShader.hlsl", VERTEX_COLOR);
    worldBuffer = new MatrixBuffer();
}

BaseLine::~BaseLine()
{
    delete mesh;
    delete worldBuffer;
}

void BaseLine::Update()
{
}

void BaseLine::Render()
{
    worldBuffer->Set(world);
    worldBuffer->SetVS(0);

    material->Set();

    mesh->Draw(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
}

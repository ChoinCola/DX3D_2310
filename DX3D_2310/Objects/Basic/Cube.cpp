#include "Framework.h"

Cube::Cube(Float3 size)
{
	vertexShader = new VertexShader(L"Shaders/Tutorial.hlsl");
	pixelShader = new PixelShader(L"Shaders/Tutorial.hlsl");

    vertices.emplace_back(-1, -1, -1, 1, 0, 0);
    vertices.emplace_back(-1, +1, -1, 0, 1, 0);
    vertices.emplace_back(+1, -1, -1, 0, 0, 1);
    vertices.emplace_back(+1, +1, -1, 1, 1, 0);

    vertices.emplace_back(-1, -1, +1, 1, 0, 1);
    vertices.emplace_back(-1, +1, +1, 0, 1, 1);
    vertices.emplace_back(+1, -1, +1, 1, 1, 1);
    vertices.emplace_back(+1, +1, +1, 0, 0, 0);

    vertexBuffer = new VertexBuffer(vertices.data(), 
        sizeof(VertexColor), vertices.size());

    indices =
    {
        // Front
        0, 1, 2, 2, 1, 3,
        // UP
        1, 5, 3, 3, 5, 7,
        // Left
        0, 4, 1, 1, 4, 5,
        // Right
        2, 3, 6, 6, 3, 7,
        // Down
        0, 2, 4, 4, 2, 6,
        // Back
        6, 7, 5, 5, 4, 6
    };
    indexBuffer = new IndexBuffer(indices.data(), indices.size());

    worldBuffer = new MatrixBuffer();
}

Cube::~Cube()
{
    delete vertexShader;
    delete pixelShader;
    delete vertexBuffer;
    delete indexBuffer;
    delete worldBuffer;
}

void Cube::Update()
{
    // SRT
    worldBuffer->Set(XMMatrixTranslation(pos.x, pos.y, pos.z));
}

void Cube::Render()
{
    vertexBuffer->Set();
    indexBuffer->Set();

    worldBuffer->SetVS(0);

    vertexShader->Set();
    pixelShader->Set();

    DC->DrawIndexed(indices.size(), 0, 0);
}

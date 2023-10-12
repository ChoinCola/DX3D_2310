#include "Framework.h"

BaseLine::BaseLine(float size, int Maxlinesize)
{
    float lineSize = Maxlinesize * 0.5;
    for (int i = 0; i <= Maxlinesize / size; i ++) {
        vertices.emplace_back(-lineSize + i * size, 0.0f, +lineSize,  0.25, 0.25, 0.25);
        vertices.emplace_back(-lineSize + i * size, 0.0f, -lineSize,  0.25, 0.25, 0.25);
        vertices.emplace_back(-lineSize, 0.0f, -lineSize + i * size, 0.25, 0.25, 0.25);
        vertices.emplace_back(+lineSize, 0.0f, -lineSize + i * size, 0.25, 0.25, 0.25);


        //01 23 45 67 89
    }   
    // 마지막 Vertex의 위치값 또한 입력 +2
    for (int i = 0; i <= Maxlinesize / size * 4 + 2; i+=2) {
        indices.emplace_back(i);
        indices.emplace_back(i + 1);
    }

    vertexShader = new VertexShader(L"Shaders/Tutorial.hlsl");
    pixelShader = new PixelShader(L"Shaders/Tutorial.hlsl");

    vertexBuffer = new VertexBuffer(vertices.data(), sizeof(VertexColor), vertices.size());
    indexBuffer = new IndexBuffer(indices.data(), indices.size());

    worldBuffer = new MatrixBuffer();

}

BaseLine::~BaseLine()
{
    delete vertexShader;
    delete pixelShader;
    delete vertexBuffer;
    delete indexBuffer;
    delete worldBuffer;
}

void BaseLine::Update()
{
}

void BaseLine::Render()
{
    worldBuffer->Set(world);
    worldBuffer->SetVS(0);

    vertexBuffer->Set(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
    indexBuffer->Set();

    vertexShader->Set();
    pixelShader->Set();

    DC->DrawIndexed(indices.size(), 0, 0);
}

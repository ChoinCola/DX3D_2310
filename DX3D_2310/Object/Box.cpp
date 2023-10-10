#include "Framework.h"
#include "Box.h"

Box::Box(int size, XMFLOAT3 position)
{
	// Create Vertex
	{
        vertices.emplace_back(-1, -1, -1, 1, 0, 0);
        vertices.emplace_back(-1, +1, -1, 0, 1, 0);
        vertices.emplace_back(+1, -1, -1, 0, 0, 1);
        vertices.emplace_back(+1, +1, -1, 1, 1, 0);

        vertices.emplace_back(-1, -1, +1, 1, 0, 1);
        vertices.emplace_back(-1, +1, +1, 0, 1, 1);
        vertices.emplace_back(+1, -1, +1, 1, 1, 1);
        vertices.emplace_back(+1, +1, +1, 0, 0, 0);
	}

    // Resize
    for (auto def : vertices) {
        def.pos.x *= size;
        def.pos.y *= size;
        def.pos.z *= size;
    }

    // Repositon
    for (auto def : vertices) {
        def.pos.x += position.x;
        def.pos.y += position.y;
        def.pos.z += position.z;
    }

    // Create Indices
    indices.insert(indices.end(),
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
        });

    // Create Buffer
    UINT verticesSize = sizeof(Vertex) * vertices.size();
    vertexBuffer = new Buffer(D3D11_BIND_VERTEX_BUFFER, verticesSize, D3D11_USAGE_DEFAULT, vertices.data());

    UINT indicesSize = sizeof(UINT) * indices.size();
    IndexBuffer = new Buffer(D3D11_BIND_INDEX_BUFFER, indicesSize, D3D11_USAGE_DEFAULT, indices.data());

    // Set Shader
    DWORD flags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG;
    shader = new Shader(L"Shaders/Tutorial.hlsl", flags);
}

Box::Box(int size, XMFLOAT3 position, XMFLOAT3 color)
{
    // Create Vertex
    {
        vertices.emplace_back(-1, -1, -1, color);
        vertices.emplace_back(-1, +1, -1, color);
        vertices.emplace_back(+1, -1, -1, color);
        vertices.emplace_back(+1, +1, -1, color);

        vertices.emplace_back(-1, -1, +1, color);
        vertices.emplace_back(-1, +1, +1, color);
        vertices.emplace_back(+1, -1, +1, color);
        vertices.emplace_back(+1, +1, +1, color);
    }

    // Resize
    for (auto def : vertices) {
        def.pos.x *= size;
        def.pos.y *= size;
        def.pos.z *= size;
    }

    // Repositon
    for (auto def : vertices) {
        def.pos.x += position.x;
        def.pos.y += position.y;
        def.pos.z += position.z;
    }

    // Create Indices
    indices.insert(indices.end(),
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
        });

    // Create Buffer
    UINT verticesSize = sizeof(Vertex) * vertices.size();
    vertexBuffer = new Buffer(D3D11_BIND_VERTEX_BUFFER, verticesSize, D3D11_USAGE_DEFAULT, vertices.data());

    UINT indicesSize = sizeof(UINT) * indices.size();
    IndexBuffer = new Buffer(D3D11_BIND_INDEX_BUFFER, indicesSize, D3D11_USAGE_DEFAULT, indices.data());

    // Set Shader
    DWORD flags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG;
    shader = new Shader(L"Shaders/Tutorial.hlsl", flags);
}

Box::~Box()
{
    Delete();
}

void Box::Update()
{
    int i;
}

void Box::Render()
{
    DC->IASetInputLayout(shader->GetinputLayout());
    DC->IASetVertexBuffers(0, 1, vertexBuffer->GetBuffer(), vertexBuffer->Getstride(), vertexBuffer->Getoffset());
    DC->IASetIndexBuffer(*IndexBuffer->GetBuffer(), DXGI_FORMAT_R32_UINT, 0); // 인덱스 버퍼 세팅법

    DC->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // 선 그림
    DC->VSSetShader(shader->GetvertexShader(), nullptr, 0);
    DC->PSSetShader(shader->GetpixelShader(), nullptr, 0);

    DC->DrawIndexed(36, 0, 0);
}

void Box::Delete()
{
    delete vertexBuffer;
    delete IndexBuffer;
    delete shader;
}

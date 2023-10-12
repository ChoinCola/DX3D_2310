#include "Framework.h"

Cube::Cube(Float3 size)
{
    Float3 halfSize(size.x * 0.5, size.y * 0.5, size.z * 0.5);

    vertices.emplace_back(-halfSize.x, -halfSize.y, -halfSize.z, 1, 0, 0);
    vertices.emplace_back(-halfSize.x, +halfSize.y, -halfSize.z, 0, 1, 0);
    vertices.emplace_back(+halfSize.x, -halfSize.y, -halfSize.z, 0, 0, 1);
    vertices.emplace_back(+halfSize.x, +halfSize.y, -halfSize.z, 1, 1, 0);

    vertices.emplace_back(-halfSize.x, -halfSize.y, +halfSize.z, 1, 0, 1);
    vertices.emplace_back(-halfSize.x, +halfSize.y, +halfSize.z, 0, 1, 1);
    vertices.emplace_back(+halfSize.x, -halfSize.y, +halfSize.z, 1, 1, 1);
    vertices.emplace_back(+halfSize.x, +halfSize.y, +halfSize.z, 0, 0, 0);

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

    vertexShader = new VertexShader(L"Shaders/Tutorial.hlsl");
    pixelShader = new PixelShader(L"Shaders/Tutorial.hlsl");

    vertexBuffer = new VertexBuffer(vertices.data(), sizeof(VertexColor), vertices.size());
    indexBuffer = new IndexBuffer(indices.data(), indices.size());

    worldBuffer = new MatrixBuffer();
    //matworld._11 = 1;
    //matworld._22 = 1;
    //matworld._33 = 1;
    //matworld._44 = 1;
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
    //if (KEYBOARD->Press('W'))
    //    matworld._43 += DELTA;
    //if (KEYBOARD->Press('S'))
    //    matworld._43 -= DELTA;
    //if (KEYBOARD->Press('A'))
    //    matworld._41 -= DELTA;
    //if (KEYBOARD->Press('D'))
    //    matworld._41 += DELTA;
    //if (KEYBOARD->Press('Q'))
    //    matworld._42 += DELTA;
    //if (KEYBOARD->Press('E'))
    //    matworld._42 -= DELTA;

    //if (KEYBOARD->Press('T'))
    //    matworld._33 += DELTA;
    //if (KEYBOARD->Press('G'))
    //    matworld._33 -= DELTA;
    //if (KEYBOARD->Press('F'))
    //    matworld._11 -= DELTA;
    //if (KEYBOARD->Press('H'))
    //    matworld._11 += DELTA;
    //if (KEYBOARD->Press('R'))
    //    matworld._22 += DELTA;
    //if (KEYBOARD->Press('Y'))
    //    matworld._22 -= DELTA;


    //static float angle = 0.0f;
    //if (KEYBOARD->Press(VK_UP))
    //    angle += DELTA;
    //if (KEYBOARD->Press(VK_DOWN))
    //    angle -= DELTA;

    //matworld._22 = cos(angle);
    //matworld._23 = sin(angle);
    //matworld._32 = -sin(angle);
    //matworld._33 = cos(angle);


    //world = XMLoadFloat4x4(&matworld);
    //worldBuffer->Set(world);
}

void Cube::Render()
{
    worldBuffer->Set(world);
    worldBuffer->SetVS(0);

    vertexBuffer->Set();
    indexBuffer->Set();

    vertexShader->Set();
    pixelShader->Set();

    DC->DrawIndexed(indices.size(), 0, 0);
}

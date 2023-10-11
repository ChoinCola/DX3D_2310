#include "Framework.h"

Cube::Cube(Float3 size)
    : size(size)
{
    key = Keyboard::Get();

    vertices.emplace_back(-1, -1, -1, 1, 0, 0);
    vertices.emplace_back(-1, +1, -1, 0, 1, 0);
    vertices.emplace_back(+1, -1, -1, 0, 0, 1);
    vertices.emplace_back(+1, +1, -1, 1, 1, 0);

    vertices.emplace_back(-1, -1, +1, 1, 0, 1);
    vertices.emplace_back(-1, +1, +1, 0, 1, 1);
    vertices.emplace_back(+1, -1, +1, 1, 1, 1);
    vertices.emplace_back(+1, +1, +1, 0, 0, 0);

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
    Move();
    Resize();
    Rotation();
    SRT();
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

void Cube::Move()
{
    if (KEYBOARD->Press('W')) { pos.z += +0.001; }
    if (KEYBOARD->Press('S')) { pos.z += -0.001; }
    if (KEYBOARD->Press('A')) { pos.x += +0.001; }
    if (KEYBOARD->Press('D')) { pos.x += -0.001; }
    if (KEYBOARD->Press('Q')) { pos.y += +0.001; }
    if (KEYBOARD->Press('E')) { pos.y += -0.001; }
}

void Cube::Resize()
{
    if (KEYBOARD->Press(VK_NUMPAD8)) { size.z += +0.001; }
    if (KEYBOARD->Press(VK_NUMPAD5)) { size.z += -0.001; }
    if (KEYBOARD->Press(VK_NUMPAD4)) { size.x += +0.001; }
    if (KEYBOARD->Press(VK_NUMPAD6)) { size.x += -0.001; }
    if (KEYBOARD->Press(VK_NUMPAD7)) { size.y += +0.001; }
    if (KEYBOARD->Press(VK_NUMPAD9)) { size.y += -0.001; }
}

void Cube::Rotation()
{
    if (KEYBOARD->Press('I')) { rotation.z += +0.001; }
    if (KEYBOARD->Press('K')) { rotation.z += -0.001; }
    if (KEYBOARD->Press('J')) { rotation.x += +0.001; }
    if (KEYBOARD->Press('L')) { rotation.x += -0.001; }
    if (KEYBOARD->Press('U')) { rotation.y += +0.001; }
    if (KEYBOARD->Press('O')) { rotation.y += -0.001; }
}

void Cube::SRT()
{
    // SRT
    S = XMMatrixScaling(size.x, size.y, size.z);
    R = XMMatrixRotationX(rotation.x) * XMMatrixRotationY(rotation.y) * XMMatrixRotationZ(rotation.z);
    T = XMMatrixTranslation(pos.x, pos.y, pos.z);
    world = S * R * T;

    worldBuffer->Set(world);
}

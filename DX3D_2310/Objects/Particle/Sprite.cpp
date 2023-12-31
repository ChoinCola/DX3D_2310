#include "Framework.h"

Sprite::Sprite(wstring imageFile, float width, float height, float frameCol, float frameRow, bool isAdditive)
    : size(width, height)
{
    material->SetShader(L"Geometry/Sprite.hlsl");
    material->SetDiffuseMap(imageFile);

    geometryShader = Shader::AddGS(L"Geometry/Sprite.hlsl");

    buffer = new FloatValueBuffer();
    buffer->GetData()[0] = frameCol;
    buffer->GetData()[1] = frameRow;

    maxFrameCount = frameCol * frameRow;

    if (isAdditive)
        blendState[1]->Additive();

    Create();
}

Sprite::~Sprite()
{
    delete buffer;
}

void Sprite::Update()
{
    if (!isActive) return;

    time += speed * DELTA;

    if (time > 0.1f)
    {
        curFrameCount++;
        buffer->GetData()[2] = curFrameCount % (UINT)buffer->GetData()[0];
        buffer->GetData()[3] = curFrameCount / (UINT)buffer->GetData()[0];

        time = 0.0f;
    }

    if (curFrameCount >= maxFrameCount)
        Stop();
}

void Sprite::Render()
{
    buffer->SetPS(10);

    Particle::Render();
}

void Sprite::GUIRender()
{
}

void Sprite::Play(Vector3 position)
{
    isActive = true;
    
    time = 0.0f;
    curFrameCount = 0;

    vertex.pos = position;
    vertex.uv = size;

    vertexBuffer->Update(&vertex, particleCount);
}

void Sprite::Create()
{
    particleCount = 1;

    vertexBuffer = new VertexBuffer(&vertex, sizeof(VertexUV), 1);
}

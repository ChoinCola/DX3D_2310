#include "Framework.h"

Spark::Spark(wstring imageFile, bool isAdditive)
{
    material->SetShader(L"Geometry/Spark.hlsl");
    material->SetDiffuseMap(imageFile);

    geometryShader = Shader::AddGS(L"Geometry/Spark.hlsl");

    buffer = new FloatValueBuffer();    
    startColorbuffer = new FloatValueBuffer();
    endColorbuffer = new FloatValueBuffer();

    if (isAdditive)
        blendState[1]->Additive();

    Create();
}

Spark::~Spark()
{
    delete buffer;
}

void Spark::Update()
{
    if (!isActive)return;

    buffer->GetData()[0] += DELTA;

    if (buffer->GetData()[0] > buffer->GetData()[1])
        Stop();
}

void Spark::Render()
{
    buffer->SetVS(10);
    buffer->SetPS(10);
    startColorbuffer->SetPS(11);
    endColorbuffer->SetPS(12);

    Particle::Render();
}

void Spark::GUIRender()
{
    ImGui::Text("Spark Option");
    ImGui::SliderInt("ParticleCount", (int*)&particleCount, 1, SIZE);
    ImGui::ColorEdit4("StartColor", startColorbuffer->GetData());
    ImGui::ColorEdit4("EndColor", endColorbuffer->GetData());
}

void Spark::Play(Vector3 position)
{
    buffer->GetData()[0] = 0.0f;

    Particle::Play(position);

    UpdateParticle();
    vertexBuffer->Update(vertices.data(), particleCount);
}

void Spark::Create()
{
    vertices.resize(SIZE);
    vertexBuffer = new VertexBuffer(vertices.data(), sizeof(VertexParticle), SIZE);

    particleCount = 500;
    buffer->GetData()[1] = 1.0f;
}

void Spark::UpdateParticle()
{
    FOR(particleCount)
    {
        vertices[i].position = position;

        float size = MATH->Random(minSize, maxSize);
        vertices[i].size = { size, size };

        float radius = MATH->Random(minRadius, maxRadius);

        Float3 rot;
        rot.x = MATH->Random(0.0f, XM_2PI);
        rot.y = MATH->Random(0.0f, XM_2PI);
        rot.z = MATH->Random(0.0f, XM_2PI);

        Vector3 velocity = Vector3(0, 0, radius);

        Matrix matRot = XMMatrixRotationRollPitchYaw(rot.x, rot.y, rot.z);
        XMStoreFloat3(&vertices[i].velocity, XMVector3TransformCoord(velocity, matRot));
    }
}


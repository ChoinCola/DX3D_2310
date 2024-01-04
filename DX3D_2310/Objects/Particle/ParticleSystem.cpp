#include "Framework.h"

ParticleSystem::ParticleSystem(string file)
{
    Load(file);

    instanceBuffer = new VertexBuffer(instances.data(), sizeof(Matrix), data.count);

    blendState[0] = new BlendState();
    blendState[1] = new BlendState();
    blendState[1]->Alpha(true);

    depthState[0] = new DepthStencilState();
    depthState[1] = new DepthStencilState();
    depthState[1]->DepthWriteMask(D3D11_DEPTH_WRITE_MASK_ZERO);
}

ParticleSystem::~ParticleSystem()
{
    delete quad;
    delete instanceBuffer;

    delete blendState[0];
    delete blendState[1];

    delete depthState[0];
    delete depthState[1];
}

void ParticleSystem::Update()
{
    if (!IsActive()) return;

    lifeTime += DELTA;

    UpdatePhysical();
    UpdateColor();
    quad->UpdateWorld();

    if (lifeTime > data.duration)
    {
        if (data.isLoop)
            Init();
        else
            Stop();
    }
}

void ParticleSystem::Render()
{
    if (!IsActive()) return;

    instanceBuffer->Set(1);

    quad->SetRender();

    blendState[1]->SetState();
    depthState[1]->SetState();

    DC->DrawIndexedInstanced(6, drawCount, 0, 0, 0);

    blendState[0]->SetState();
    depthState[0]->SetState();
}

void ParticleSystem::Play(Vector3 pos, Vector3 rot)
{
    quad->SetActive(true);

    quad->SetLocalPosition(pos);
    quad->SetLocalRotation(rot);

    Init();
}

void ParticleSystem::Stop()
{
    quad->SetActive(false);
}

void ParticleSystem::UpdatePhysical()
{
    drawCount = 0;

    FOR(data.count)
    {
        if (particleInfos[i].startTime > lifeTime) continue;

        particleInfos[i].velocity += particleInfos[i].accelation * DELTA;
        particleInfos[i].transform.Translate(
            particleInfos[i].velocity * particleInfos[i].speed * DELTA);
        particleInfos[i].transform.Rotate(
            Vector3::Forward() * particleInfos[i].angularVelocity * DELTA);
        if (data.isBillboard)
        {
            Vector3 rot = CAM->GetLocalRotation();
            rot.z = particleInfos[i].transform.GetLocalRotation().z;
            particleInfos[i].transform.SetLocalRotation(rot);
        }

        float t = (lifeTime - particleInfos[i].startTime)
            / (data.duration - particleInfos[i].startTime);

        Vector3 scale = MATH->Lerp(particleInfos[i].startScale, particleInfos[i].endScale, t);
        particleInfos[i].transform.SetLocalScale(scale);

        particleInfos[i].transform.UpdateWorld();
        instances[drawCount++] = XMMatrixTranspose(particleInfos[i].transform.GetWorld());
    }

    instanceBuffer->Update(instances.data(), drawCount);
}

void ParticleSystem::UpdateColor()
{
    float t = lifeTime / data.duration;

    Float4 color;
    color.x = MATH->Lerp(data.startColor.x, data.endColor.x, t);
    color.y = MATH->Lerp(data.startColor.y, data.endColor.y, t);
    color.z = MATH->Lerp(data.startColor.z, data.endColor.z, t);
    color.w = MATH->Lerp(data.startColor.w, data.endColor.w, t);

    quad->GetMaterial()->GetBuffer()->diffuse = color;
}

void ParticleSystem::Init()
{
    if (data.isAdditive)
        blendState[1]->Additive();
    else
        blendState[1]->Alpha(true);

    lifeTime = 0.0f;
    drawCount = 0;
    data.count = particleCount;

    instances.resize(data.count);
    particleInfos.resize(data.count);

    for (ParticleInfo& info : particleInfos)
    {
        info.transform.SetLocalPosition(MATH->Random(data.minStartPos, data.maxStartPos));
        info.velocity = MATH->Random(data.minVelocity, data.maxVelocity);
        info.accelation = MATH->Random(data.minAccelation, data.maxAccelation);
        info.angularVelocity = MATH->Random(data.minAngularVelocity, data.maxAngularVelocity);
        info.speed = MATH->Random(data.minSpeed, data.maxSpeed);
        info.startTime = MATH->Random(data.minStartTime, data.maxStartTime);
        info.startScale = MATH->Random(data.minStartScale, data.maxStartScale);
        info.endScale = MATH->Random(data.minEndScale, data.maxEndScale);
        info.velocity = MATH->Random(data.minVelocity, data.maxVelocity);
        info.velocity.Normalized();
    }
}

void ParticleSystem::Load(string file)
{
    BinaryReader* reader = new BinaryReader(file);

    wstring textureFile = reader->WString();
    quad = new Quad(Float2(1, 1));
    quad->GetMaterial()->SetDiffuseMap(textureFile);
    quad->GetMaterial()->SetShader(L"Instancing/Particle.hlsl");

    ParticleData* particleData = new ParticleData();
    reader->Byte((void**)&particleData, sizeof(ParticleData));

    data = *particleData;

    instances.resize(data.count);
    particleInfos.resize(data.count);

    delete reader;
}

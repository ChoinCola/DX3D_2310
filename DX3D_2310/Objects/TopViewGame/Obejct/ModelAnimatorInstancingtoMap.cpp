#include "Framework.h"

ModelAnimatorInstancingtoMap::ModelAnimatorInstancingtoMap(string name)
    : ModelAnimator(name)
{
    SetShader(L"Instancing/ModelAnimatorInstancing.hlsl");

    instanceBuffer = new VertexBuffer(instanceDatas, sizeof(InstanceData),
        MAX_INSTANCE);

    frameInstancingBuffer = new FrameInstancingBuffer();
}

ModelAnimatorInstancingtoMap::~ModelAnimatorInstancingtoMap()
{
    for (auto vt = transforms.begin(); vt != transforms.end(); vt++)
        for (auto def : (*vt).second)
            delete def;

    for (auto vt = instanceBuffer.begin(); vt != instanceBuffer.end(); vt++)
            delete (*vt).second;

    delete frameInstancingBuffer;

}

void ModelAnimatorInstancingtoMap::Update(pair<float, float> nowpos)
{
    drawCount = 0;

    FOR(transforms[nowpos].size())
    {
        if (transforms[nowpos][i]->IsActive())
        {
            UpdateFrame(&frameInstancingBuffer->GetData()->motions[i]);
            transforms[nowpos][i]->UpdateWorld();
            instanceDatas[nowpos][drawCount].world =
                XMMatrixTranspose(transforms[nowpos][i]->GetWorld());
            instanceDatas[nowpos][drawCount].index = i;

            drawCount++;
        }
    }

    instanceBuffer[nowpos]->Update(instanceDatas[nowpos], drawCount);
}

void ModelAnimatorInstancingtoMap::Render(pair<float, float> nowpos, Float2 size)
{
    instanceBuffer->Set(1);
    frameInstancingBuffer->SetVS(4);
    DC->VSSetShaderResources(0, 1, &srv);

    for (ModelMesh* mesh : meshes)
        mesh->RenderInstanced(drawCount);
}

void ModelAnimatorInstancingtoMap::GUIRender()
{
    ImGui::Text("DrawCount : %d", drawCount);

    for (Transform* transform : transforms)
        transform->GUIRender();
}

Transform* ModelAnimatorInstancingtoMap::Add(pair<float, float> nowpos)
{
    Transform* transform = new Transform();
    transform->SetTag(name + "_" + to_string(transforms.size()));
    transforms[nowpos].push_back(transform);

    return transform;
}

void ModelAnimatorInstancingtoMap::PlayClip(UINT instanceInex, int clip, float scale, float takeTime)
{
    frameInstancingBuffer->GetData()->motions[instanceInex].next.clip = clip;
    frameInstancingBuffer->GetData()->motions[instanceInex].next.scale = scale;
    frameInstancingBuffer->GetData()->motions[instanceInex].takeTime = takeTime;
    frameInstancingBuffer->GetData()->motions[instanceInex].runningTime = 0.0f;
}

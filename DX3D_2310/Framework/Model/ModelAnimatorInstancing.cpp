#include "Framework.h"
#include "ModelAnimatorInstancing.h"

ModelAnimatorInstancing::ModelAnimatorInstancing(string name)
    : ModelAnimator(name)
{
    SetShader(L"Instancing/ModelAnimatorInstancing.hlsl");

    instanceBuffer = new VertexBuffer(instanceDatas, sizeof(InstanceData),
        MAX_INSTANCE);

    frameInstancingBuffer = new FrameInstancingBuffer();
}

ModelAnimatorInstancing::~ModelAnimatorInstancing()
{
    delete instanceBuffer;
    delete frameInstancingBuffer;

    for (Transform* transform : transforms)
        delete transform;
}

void ModelAnimatorInstancing::Update()
{
    drawCount = 0;

    if (inputTransforms != nullptr)
    {
        FOR(inputTransforms->size())
        {
            if ((*inputTransforms)[i]->IsActive())
            {
                UpdateFrame(&frameInstancingBuffer->GetData()->motions[i]);
                (*inputTransforms)[i]->UpdateWorld();
                instanceDatas[drawCount].world =
                    XMMatrixTranspose(transforms[i]->GetWorld());
                instanceDatas[drawCount].index = i;

                drawCount++;
            }
        }
        instanceBuffer->Update(instanceDatas, drawCount);
        return;
    }


    FOR(transforms.size())
    {
        if (transforms[i]->IsActive())
        {
            UpdateFrame(&frameInstancingBuffer->GetData()->motions[i]);
            transforms[i]->UpdateWorld();
            instanceDatas[drawCount].world =
                XMMatrixTranspose(transforms[i]->GetWorld());
            instanceDatas[drawCount].index = i;

            drawCount++;
        }
    }

    instanceBuffer->Update(instanceDatas, drawCount);
}

void ModelAnimatorInstancing::Render()
{
    instanceBuffer->Set(1);
    frameInstancingBuffer->SetVS(4);
    DC->VSSetShaderResources(0, 1, &srv);

    for (ModelMesh* mesh : meshes)
        mesh->RenderInstanced(drawCount);
}

void ModelAnimatorInstancing::Render(VertexBuffer* input)
{
    input->Set(1);
    frameInstancingBuffer->SetVS(4);
    DC->VSSetShaderResources(0, 1, &srv);

    for (ModelMesh* mesh : meshes)
        mesh->RenderInstanced(drawCount);
}
void ModelAnimatorInstancing::GUIRender()
{
    ImGui::Text("DrawCount : %d", drawCount);

    for (Transform* transform : transforms)
        transform->GUIRender();
}

Transform* ModelAnimatorInstancing::Add()
{
    Transform* transform = new Transform();
    transform->SetTag(name + "_" + to_string(transforms.size()));
    transforms.push_back(transform);

    return transform;
}


void ModelAnimatorInstancing::SetTransforms(vector<Transform*>* transform)
{
    inputTransforms = transform;
}

void ModelAnimatorInstancing::PlayClip(UINT instanceInex, int clip, float scale, float takeTime)
{
    frameInstancingBuffer->GetData()->motions[instanceInex].next.clip = clip;
    frameInstancingBuffer->GetData()->motions[instanceInex].next.scale = scale;
    frameInstancingBuffer->GetData()->motions[instanceInex].takeTime = takeTime;
    frameInstancingBuffer->GetData()->motions[instanceInex].runningTime = 0.0f;
}

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

    if (inputTransforms.size() != 0)
    {
        FOR(inputTransforms.size())
        {
            if (inputTransforms[i]->IsActive())
            {
                UpdateFrame(&frameInstancingBuffer->GetData()->motions[i]);
                inputTransforms[i]->UpdateWorld();
                instanceDatas[drawCount].world =
                    XMMatrixTranspose(inputTransforms[i]->GetWorld());
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

    ModelAnimator::GUIRender();
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

void ModelAnimatorInstancing::TransformUnActive(UINT input)
{
    transforms[input]->SetActive(false);
}

UINT ModelAnimatorInstancing::GetLastObjectNum()
{
    return LastInputnum;
}


void ModelAnimatorInstancing::SetTransforms(vector<Transform*> transform)
{
    inputTransforms.resize(0);
    inputTransforms.reserve(transform.size());

    for (auto& def : transform) {
        inputTransforms.emplace_back(def);
    }
}

void ModelAnimatorInstancing::PlayClip(UINT instanceInex, int clip, float scale, float takeTime)
{
    ModelClip* modelClip = clips[clip];

    frameInstancingBuffer->GetData()->motions[instanceInex].next.clip = clip;
    frameInstancingBuffer->GetData()->motions[instanceInex].next.scale = scale;
    frameInstancingBuffer->GetData()->motions[instanceInex].takeTime = takeTime;
    frameInstancingBuffer->GetData()->motions[instanceInex].runningTime = 0.0f;
    frameInstancingBuffer->GetData()->motions[instanceInex].duration 
        = modelClip->frameCount / modelClip->tickPerSecond;
}

UINT ModelAnimatorInstancing::GetPlayClip(UINT instanceInex)
{
    return frameInstancingBuffer->GetData()->motions[instanceInex].cur.clip;
}

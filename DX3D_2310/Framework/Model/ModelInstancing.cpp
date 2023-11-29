#include "Framework.h"

ModelInstancing::ModelInstancing(string name) : Model(name)
{
    SetShader(L"Instancing/ModelInstancing.hlsl");

    instanceBuffer = new VertexBuffer(instanceDatas, sizeof(InstanceData),
        MAX_INSTANCE);

}

ModelInstancing::~ModelInstancing()
{
    delete instanceBuffer;
    for (Transform* transform : transforms)
        delete transform;
}

void ModelInstancing::Update()
{
    drawCount = 0;

    FOR(transforms.size())
    {
        if (transforms[i]->IsActive())
        {
            transforms[i]->UpdateWorld();
            instanceDatas[drawCount].world = XMMatrixTranspose(transforms[i]->GetWorld());

            drawCount++;
        }
    }

    instanceBuffer->Update(instanceDatas, drawCount);
}

void ModelInstancing::Render()
{
    instanceBuffer->Set(1);

    for (ModelMesh* mesh : meshes)
        mesh->RenderInstanced(drawCount);

}

void ModelInstancing::GUIRender()
{
    ImGui::Text("DrawCount : %d", drawCount);

    for (Transform* transform : transforms)
        transform->GUIRender();
}

Transform* ModelInstancing::Add()
{
    Transform* transform = new Transform();
    transform->SetTag(name + "_" + to_string(transforms.size()));
    transforms.push_back(transform);

    return transform;
}

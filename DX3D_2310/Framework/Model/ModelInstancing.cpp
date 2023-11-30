#include "Framework.h"
// ModelInstancing 클래스의 생성자
// 부모 클래스인 Model의 생성자를 호출하고, 인스턴싱에 필요한 셰이더를 설정하며, 인스턴스 버퍼를 초기화합니다.
ModelInstancing::ModelInstancing(string name) : Model(name)
{
    // Model 클래스의 생성자 호출
    SetShader(L"Instancing/ModelInstancing.hlsl");

    // 인스턴스 버퍼를 초기화합니다.
    instanceBuffer = new VertexBuffer(instanceDatas, sizeof(InstanceData), MAX_INSTANCE);
}

// ModelInstancing 클래스의 소멸자
// 인스턴스 버퍼 및 관련 트랜스폼을 메모리에서 해제합니다.
ModelInstancing::~ModelInstancing()
{
    delete instanceBuffer;

    // 트랜스폼들을 순회하면서 메모리에서 해제합니다.
    for (Transform* transform : transforms)
        delete transform;
}

// 인스턴스들의 월드 행렬을 업데이트하는 함수
void ModelInstancing::Update()
{
    // 그려질 인스턴스 수를 초기화합니다.
    drawCount = 0;

    // 트랜스폼들을 순회하면서 활성화된 트랜스폼의 월드 행렬을 업데이트하고, 인스턴스 데이터에 저장합니다.
    FOR(transforms.size())
    {
        if (transforms[i]->IsActive())
        {
            transforms[i]->UpdateWorld();
            instanceDatas[drawCount].world = XMMatrixTranspose(transforms[i]->GetWorld());

            drawCount++;
        }
    }

    // 업데이트된 인스턴스 데이터를 인스턴스 버퍼에 전달하여 업데이트합니다.
    instanceBuffer->Update(instanceDatas, drawCount);
}

// 인스턴스 렌더링을 수행하는 함수
void ModelInstancing::Render()
{
    // 인스턴스 버퍼를 입력 어셈블러 스테이지에 설정합니다.
    instanceBuffer->Set(1);

    // 각 메시를 순회하면서 인스턴스 렌더링을 수행합니다.
    for (ModelMesh* mesh : meshes)
        mesh->RenderInstanced(drawCount);
}

// GUI 렌더링을 위한 함수
void ModelInstancing::GUIRender()
{
    // 현재 그려질 인스턴스 수를 ImGui를 통해 표시합니다.
    ImGui::Text("DrawCount : %d", drawCount);

    // 트랜스폼들을 순회하면서 각각의 GUI를 렌더링합니다.
    for (Transform* transform : transforms)
        transform->GUIRender();
}

// 새로운 트랜스폼을 추가하고 해당 트랜스폼을 반환하는 함수
Transform* ModelInstancing::Add()
{
    // 새로운 트랜스폼을 생성하고 이름을 설정합니다.
    Transform* transform = new Transform();
    transform->SetTag(name + "_" + to_string(transforms.size()));

    // 생성한 트랜스폼을 리스트에 추가합니다.
    transforms.push_back(transform);

    // 생성한 트랜스폼을 반환합니다.
    return transform;
}
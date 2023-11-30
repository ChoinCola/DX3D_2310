#include "Framework.h"
// ModelInstancing Ŭ������ ������
// �θ� Ŭ������ Model�� �����ڸ� ȣ���ϰ�, �ν��Ͻ̿� �ʿ��� ���̴��� �����ϸ�, �ν��Ͻ� ���۸� �ʱ�ȭ�մϴ�.
ModelInstancing::ModelInstancing(string name) : Model(name)
{
    // Model Ŭ������ ������ ȣ��
    SetShader(L"Instancing/ModelInstancing.hlsl");

    // �ν��Ͻ� ���۸� �ʱ�ȭ�մϴ�.
    instanceBuffer = new VertexBuffer(instanceDatas, sizeof(InstanceData), MAX_INSTANCE);
}

// ModelInstancing Ŭ������ �Ҹ���
// �ν��Ͻ� ���� �� ���� Ʈ�������� �޸𸮿��� �����մϴ�.
ModelInstancing::~ModelInstancing()
{
    delete instanceBuffer;

    // Ʈ���������� ��ȸ�ϸ鼭 �޸𸮿��� �����մϴ�.
    for (Transform* transform : transforms)
        delete transform;
}

// �ν��Ͻ����� ���� ����� ������Ʈ�ϴ� �Լ�
void ModelInstancing::Update()
{
    // �׷��� �ν��Ͻ� ���� �ʱ�ȭ�մϴ�.
    drawCount = 0;

    // Ʈ���������� ��ȸ�ϸ鼭 Ȱ��ȭ�� Ʈ�������� ���� ����� ������Ʈ�ϰ�, �ν��Ͻ� �����Ϳ� �����մϴ�.
    FOR(transforms.size())
    {
        if (transforms[i]->IsActive())
        {
            transforms[i]->UpdateWorld();
            instanceDatas[drawCount].world = XMMatrixTranspose(transforms[i]->GetWorld());

            drawCount++;
        }
    }

    // ������Ʈ�� �ν��Ͻ� �����͸� �ν��Ͻ� ���ۿ� �����Ͽ� ������Ʈ�մϴ�.
    instanceBuffer->Update(instanceDatas, drawCount);
}

// �ν��Ͻ� �������� �����ϴ� �Լ�
void ModelInstancing::Render()
{
    // �ν��Ͻ� ���۸� �Է� ����� ���������� �����մϴ�.
    instanceBuffer->Set(1);

    // �� �޽ø� ��ȸ�ϸ鼭 �ν��Ͻ� �������� �����մϴ�.
    for (ModelMesh* mesh : meshes)
        mesh->RenderInstanced(drawCount);
}

// GUI �������� ���� �Լ�
void ModelInstancing::GUIRender()
{
    // ���� �׷��� �ν��Ͻ� ���� ImGui�� ���� ǥ���մϴ�.
    ImGui::Text("DrawCount : %d", drawCount);

    // Ʈ���������� ��ȸ�ϸ鼭 ������ GUI�� �������մϴ�.
    for (Transform* transform : transforms)
        transform->GUIRender();
}

// ���ο� Ʈ�������� �߰��ϰ� �ش� Ʈ�������� ��ȯ�ϴ� �Լ�
Transform* ModelInstancing::Add()
{
    // ���ο� Ʈ�������� �����ϰ� �̸��� �����մϴ�.
    Transform* transform = new Transform();
    transform->SetTag(name + "_" + to_string(transforms.size()));

    // ������ Ʈ�������� ����Ʈ�� �߰��մϴ�.
    transforms.push_back(transform);

    // ������ Ʈ�������� ��ȯ�մϴ�.
    return transform;
}
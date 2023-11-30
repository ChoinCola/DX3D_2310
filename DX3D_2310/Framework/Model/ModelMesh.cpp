#include "Framework.h"

ModelMesh::ModelMesh(string name) : name(name)
{
    data.name = name;
    blendState[0] = new BlendState();
    blendState[1] = new BlendState();

    rasterizerstate[0] = new RasterizerState();
    rasterizerstate[1] = new RasterizerState();
}

ModelMesh::~ModelMesh()
{
    delete blendState;
    delete rasterizerstate;
	delete mesh;
}

void ModelMesh::Render()
{
	material->Set();
    
    blendState[1]->SetState();
    rasterizerstate[1]->SetState();
	mesh->Draw();

    blendState[0]->SetState();
    rasterizerstate[0]->SetState();

}

void ModelMesh::RenderInstanced(UINT count)
{
    material->Set();
    blendState[1]->SetState();
    rasterizerstate[1]->SetState();
    mesh->DrawInstanced(count);
    blendState[0]->SetState();
    rasterizerstate[0]->SetState();

}

void ModelMesh::CreateMesh(void* vertices, UINT vertexCount, void* indices, UINT indexCount)
{
    // Mesh Ŭ������ �ν��Ͻ��� �����մϴ�.
    mesh = new Mesh<ModelVertex>();

    // Mesh�� ���� (vertices) ���͸� �Ҵ��ϰ� ũ�⸦ �����մϴ�.
    // �ӵ��� ���� �̸� �������
    mesh->GetVertices().resize(vertexCount);

    // void �����ͷ� ���޵� ���� �����͸� �����Ͽ� Mesh�� ���� ���Ϳ� �ֽ��ϴ�.
    // �޸�cpy�� �Ϲ����� �����ͱ��Թ�ĺ��� �� ������.
    memcpy(mesh->GetVertices().data(), vertices, sizeof(ModelVertex) * vertexCount);

    // Mesh�� �ε��� (indices) ���͸� �Ҵ��ϰ� ũ�⸦ �����մϴ�.
    mesh->GetIndices().resize(indexCount);

    // void �����ͷ� ���޵� �ε��� �����͸� �����Ͽ� Mesh�� �ε��� ���Ϳ� �ֽ��ϴ�.
    memcpy(mesh->GetIndices().data(), indices, sizeof(UINT) * indexCount);

    // Mesh�� �����ϴ� �Լ��� ȣ���Ͽ� ���� �� �ε��� �����ͷ� Mesh�� �ʱ�ȭ�մϴ�.
    mesh->CreateMesh();

    data.vertices = mesh->GetVertices();
    data.indices = mesh->GetIndices();
}

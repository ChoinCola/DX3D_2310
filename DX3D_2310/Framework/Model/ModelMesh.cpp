#include "Framework.h"

ModelMesh::ModelMesh(string name) : name(name)
{
    data.name = name;
}

ModelMesh::~ModelMesh()
{
	delete mesh;
}

void ModelMesh::Render()
{
	material->Set();
	mesh->Draw();
}

void ModelMesh::RenderInstanced(UINT count)
{
    material->Set();
    mesh->DrawInstanced(count);
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

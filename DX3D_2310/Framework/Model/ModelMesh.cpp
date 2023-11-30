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
    // Mesh 클래스의 인스턴스를 생성합니다.
    mesh = new Mesh<ModelVertex>();

    // Mesh의 정점 (vertices) 벡터를 할당하고 크기를 설정합니다.
    // 속도를 위해 미리 사이즈설정
    mesh->GetVertices().resize(vertexCount);

    // void 포인터로 전달된 정점 데이터를 복사하여 Mesh의 정점 벡터에 넣습니다.
    // 메모리cpy가 일반적인 데이터기입방식보다 더 빠르다.
    memcpy(mesh->GetVertices().data(), vertices, sizeof(ModelVertex) * vertexCount);

    // Mesh의 인덱스 (indices) 벡터를 할당하고 크기를 설정합니다.
    mesh->GetIndices().resize(indexCount);

    // void 포인터로 전달된 인덱스 데이터를 복사하여 Mesh의 인덱스 벡터에 넣습니다.
    memcpy(mesh->GetIndices().data(), indices, sizeof(UINT) * indexCount);

    // Mesh를 생성하는 함수를 호출하여 정점 및 인덱스 데이터로 Mesh를 초기화합니다.
    mesh->CreateMesh();

    data.vertices = mesh->GetVertices();
    data.indices = mesh->GetIndices();
}

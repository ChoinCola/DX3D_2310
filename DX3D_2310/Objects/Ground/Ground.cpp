#include "Framework.h"

Ground::Ground(UINT MaxSize, UINT BlockSize)
{
    material = new Material(L"Tutorial.hlsl");
    mesh = new Mesh<VertexUV>();

    Float3 halfSize(BlockSize * 0.5, BlockSize * 0.5, BlockSize * 0.5);

    vector<VertexUV>& vertices = mesh->GetVertices();
    for(int i = 0; i < MaxSize; i++)
    //Front
    vertices.emplace_back(-halfSize.x, -halfSize.y, -halfSize.z, 0, 1);
    vertices.emplace_back(-halfSize.x, +halfSize.y, -halfSize.z, 0, 0);
    vertices.emplace_back(+halfSize.x, -halfSize.y, -halfSize.z, 1, 1);
    vertices.emplace_back(+halfSize.x, +halfSize.y, -halfSize.z, 1, 0);

    //Up
    vertices.emplace_back(-halfSize.x, +halfSize.y, -halfSize.z, 0, 1);
    vertices.emplace_back(-halfSize.x, +halfSize.y, +halfSize.z, 0, 0);
    vertices.emplace_back(+halfSize.x, +halfSize.y, -halfSize.z, 1, 1);
    vertices.emplace_back(+halfSize.x, +halfSize.y, +halfSize.z, 1, 0);

    //Right
    vertices.emplace_back(+halfSize.x, +halfSize.y, -halfSize.z, 0, 1);
    vertices.emplace_back(+halfSize.x, +halfSize.y, +halfSize.z, 0, 0);
    vertices.emplace_back(+halfSize.x, -halfSize.y, -halfSize.z, 1, 1);
    vertices.emplace_back(+halfSize.x, -halfSize.y, +halfSize.z, 1, 0);

    //Left
    vertices.emplace_back(-halfSize.x, +halfSize.y, -halfSize.z, 0, 1);
    vertices.emplace_back(-halfSize.x, -halfSize.y, -halfSize.z, 0, 0);
    vertices.emplace_back(-halfSize.x, +halfSize.y, +halfSize.z, 1, 1);
    vertices.emplace_back(-halfSize.x, -halfSize.y, +halfSize.z, 1, 0);

    //Back
    vertices.emplace_back(-halfSize.x, +halfSize.y, +halfSize.z, 0, 1);
    vertices.emplace_back(-halfSize.x, -halfSize.y, +halfSize.z, 0, 0);
    vertices.emplace_back(+halfSize.x, +halfSize.y, +halfSize.z, 1, 1);
    vertices.emplace_back(+halfSize.x, -halfSize.y, +halfSize.z, 1, 0);

    //Down
    vertices.emplace_back(-halfSize.x, -halfSize.y, -halfSize.z, 0, 1);
    vertices.emplace_back(+halfSize.x, -halfSize.y, -halfSize.z, 0, 0);
    vertices.emplace_back(-halfSize.x, -halfSize.y, +halfSize.z, 1, 1);
    vertices.emplace_back(+halfSize.x, -halfSize.y, +halfSize.z, 1, 0);

    vector<UINT>& indices = mesh->GetIndices();

    /*
        1        3
        ----------
        1        1
        1        1
        1        1
        ----------
        0        2
    */

    indices =
    {
        //Front
        0, 1, 2, 2, 1, 3,
        //Up
        4, 5, 6, 6, 5, 7,
        //Right
        8, 9, 10, 10, 9, 11,
        //Left
        12, 13, 14, 14, 13, 15,
        //Back
        16, 17, 18, 18, 17, 19,
        //Down
        20, 21, 22, 22, 21, 23
    };


    mesh->CreateMesh();

    worldBuffer = new MatrixBuffer();

}

Ground::~Ground()
{
    delete mesh;
    delete worldBuffer;
}

void Ground::Update()
{
}

void Ground::Render()
{
    worldBuffer->Set(world);
    worldBuffer->SetVS(0);

    material->Set();

    mesh->Draw();
}

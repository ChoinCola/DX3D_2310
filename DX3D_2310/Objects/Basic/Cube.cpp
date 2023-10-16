#include "Framework.h"

Cube::Cube(Float3 size, Vector3 pos)
{
    material = new Material(L"Tutorial.hlsl",VERTEX_UV);
    mesh = new Mesh<VertexUV>();

    SetLocalPosition(pos);
    Float3 halfSize(size.x * 0.5, size.y * 0.5, size.z * 0.5);

    vector<VertexUV>& vertices = mesh->GetVertices();
    // Mid;
    vertices.emplace_back(-halfSize.x, -halfSize.y, -halfSize.z, 0, 1);
    vertices.emplace_back(-halfSize.x, +halfSize.y, -halfSize.z, 0, 0);
    vertices.emplace_back(+halfSize.x, -halfSize.y, -halfSize.z, 1, 1);
    vertices.emplace_back(+halfSize.x, +halfSize.y, -halfSize.z, 1, 0);
                                                                
    vertices.emplace_back(-halfSize.x, -halfSize.y, +halfSize.z, 1, 1);
    vertices.emplace_back(-halfSize.x, +halfSize.y, +halfSize.z, 1, 0);
    vertices.emplace_back(+halfSize.x, -halfSize.y, +halfSize.z, 0, 1);
    vertices.emplace_back(+halfSize.x, +halfSize.y, +halfSize.z, 0, 0);

    // UP 
    vertices.emplace_back(-halfSize.x, +halfSize.y, +halfSize.z, 0, 1);     //5
    vertices.emplace_back(+halfSize.x, +halfSize.y, +halfSize.z, 1, 1);     //7

    // DOWN
    vertices.emplace_back(-halfSize.x, -halfSize.y, +halfSize.z, 0, 0);     //4
    vertices.emplace_back(+halfSize.x, -halfSize.y, +halfSize.z, 1, 0);     //6

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
        // Front
        0, 1, 2, 2, 1, 3,
        // Left
        0, 4, 1, 1, 4, 5,
        // Right
        2, 3, 6, 6, 3, 7,
        // Back
        6, 7, 5, 5, 4, 6,

        // UP
        1, 8, 3, 3, 8, 9,
        // Down
        0, 2, 10, 10, 2, 11,
    };

    
    mesh->CreateMesh();

    worldBuffer = new MatrixBuffer();

    ScratchImage image;
    LoadFromWICFile(L"Textures/Landscape/Box.png", WIC_FLAGS_NONE, nullptr, image);
    // srvÇÒ´ç
    CreateShaderResourceView(DEVICE, image.GetImages(), image.GetImageCount(), image.GetMetadata(), &srv);
}

Cube::~Cube()
{
    delete mesh;
    delete worldBuffer;
    srv->Release();
}

void Cube::Update()
{
    
}

void Cube::Render()
{
    worldBuffer->Set(world);
    worldBuffer->SetVS(0);
    DC->PSSetShaderResources(0, 1, &srv);
    material->Set();

    mesh->Draw();
}

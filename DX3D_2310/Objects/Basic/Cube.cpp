#include "Framework.h"

Cube::Cube(Float3 size)
{
    material = new Material(L"Tutorial.hlsl",VERTEX_UV);
    mesh = new Mesh<VertexUV>();
    Float3 halfSize(size.x * 0.5, size.y * 0.5, size.z * 0.5);

    vector<VertexUV>& vertices = mesh->GetVertices();
    vertices.emplace_back(-halfSize.x, -halfSize.y, -halfSize.z, 0, 0);
    vertices.emplace_back(-halfSize.x, +halfSize.y, -halfSize.z, 1, 0);
    vertices.emplace_back(+halfSize.x, -halfSize.y, -halfSize.z, 0, 1);
    vertices.emplace_back(+halfSize.x, +halfSize.y, -halfSize.z, 1, 0);
                                                                
    vertices.emplace_back(-halfSize.x, -halfSize.y, +halfSize.z, 0, 1);
    vertices.emplace_back(-halfSize.x, +halfSize.y, +halfSize.z, 1, 1);
    vertices.emplace_back(+halfSize.x, -halfSize.y, +halfSize.z, 1, 1);
    vertices.emplace_back(+halfSize.x, +halfSize.y, +halfSize.z, 0, 0);

    vector<UINT>& indices = mesh->GetIndices();

    indices =
    {
        // Front
        0, 1, 2, 2, 1, 3,
        // UP
        1, 5, 3, 3, 5, 7,
        // Left
        0, 4, 1, 1, 4, 5,
        // Right
        2, 3, 6, 6, 3, 7,
        // Down
        0, 2, 4, 4, 2, 6,
        // Back
        6, 7, 5, 5, 4, 6
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

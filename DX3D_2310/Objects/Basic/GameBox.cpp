#include "Framework.h"

GameBox::GameBox(float slice, float num, Float3 size, Vector3 pos)
{
    material = new Material(L"Tutorial.hlsl");
    mesh = new Mesh<VertexUV>();

    SetLocalPosition(pos);
    Float3 halfSize(size.x * 0.5, size.y * 0.5, size.z * 0.5);

    vector<VertexUV>& vertices = mesh->GetVertices();

    float UV_XB = 1 / slice * ((int)num % (int)slice);
    float UV_XF = UV_XB + 1 / slice;
    float UV_YU = 1 / slice * ((int)num / (int)slice);
    float UV_YD = UV_YU + 1 / slice;

    /*
          0 1 2
        0 ㅁㅁㅁ
        1 ㅁㅁㅁ
        2 ㅁㅁㅁ

    */ 

    // Mid;
    vertices.emplace_back(-halfSize.x, -halfSize.y, -halfSize.z, UV_XB, UV_YU);
    vertices.emplace_back(-halfSize.x, +halfSize.y, -halfSize.z, UV_XB, UV_YD);
    vertices.emplace_back(+halfSize.x, -halfSize.y, -halfSize.z, UV_XF, UV_YU);
    vertices.emplace_back(+halfSize.x, +halfSize.y, -halfSize.z, UV_XF, UV_YD);

    vertices.emplace_back(-halfSize.x, -halfSize.y, +halfSize.z, UV_XF, UV_YU);
    vertices.emplace_back(-halfSize.x, +halfSize.y, +halfSize.z, UV_XF, UV_YD);
    vertices.emplace_back(+halfSize.x, -halfSize.y, +halfSize.z, UV_XB, UV_YU);
    vertices.emplace_back(+halfSize.x, +halfSize.y, +halfSize.z, UV_XB, UV_YD);

    // UP 
    vertices.emplace_back(-halfSize.x, +halfSize.y, +halfSize.z, UV_XB, UV_YU);     //5
    vertices.emplace_back(+halfSize.x, +halfSize.y, +halfSize.z, UV_XF, UV_YU);     //7

    // DOWN
    vertices.emplace_back(-halfSize.x, -halfSize.y, +halfSize.z, UV_XB, UV_YD);     //4
    vertices.emplace_back(+halfSize.x, -halfSize.y, +halfSize.z, UV_XF, UV_YD);     //6

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
    // srv할당
    CreateShaderResourceView(DEVICE, image.GetImages(),
        image.GetImageCount(), image.GetMetadata(), &srv);
}

GameBox::~GameBox()
{
    delete mesh;
    delete worldBuffer;

    srv->Release();
}

void GameBox::Update()
{
}

void GameBox::Render()
{
    worldBuffer->Set(world);
    worldBuffer->SetVS(0);
    DC->PSSetShaderResources(0, 1, &srv);
    material->Set();

    mesh->Draw();
}

void GameBox::SetImage(wstring imagenow)
{
    ScratchImage image;
    LoadFromWICFile(imagenow.c_str(), WIC_FLAGS_NONE, nullptr, image);
    // srv할당
    CreateShaderResourceView(DEVICE, image.GetImages(),
        image.GetImageCount(), image.GetMetadata(), &srv);
}

#include "Framework.h"

GameBox::GameBox(float slice, float num, Float3 size, Vector3 pos)
{
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
        0 けけけ
        1 けけけ
        2 けけけ

    */ 

    // Mid;
    vertices.emplace_back(-halfSize.x, -halfSize.y, -halfSize.z, UV_XB, UV_YD);
    vertices.emplace_back(-halfSize.x, +halfSize.y, -halfSize.z, UV_XB, UV_YU);
    vertices.emplace_back(+halfSize.x, -halfSize.y, -halfSize.z, UV_XF, UV_YD);
    vertices.emplace_back(+halfSize.x, +halfSize.y, -halfSize.z, UV_XF, UV_YU);

    vertices.emplace_back(-halfSize.x, -halfSize.y, +halfSize.z, UV_XF, UV_YD);
    vertices.emplace_back(-halfSize.x, +halfSize.y, +halfSize.z, UV_XF, UV_YU);
    vertices.emplace_back(+halfSize.x, -halfSize.y, +halfSize.z, UV_XB, UV_YD);
    vertices.emplace_back(+halfSize.x, +halfSize.y, +halfSize.z, UV_XB, UV_YU);

    // UP 
    vertices.emplace_back(-halfSize.x, +halfSize.y, +halfSize.z, UV_XB, UV_YD);     //5
    vertices.emplace_back(+halfSize.x, +halfSize.y, +halfSize.z, UV_XF, UV_YD);     //7

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
}

GameBox::~GameBox()
{
    delete mesh;
}

void GameBox::Update()
{
    UpdateWorld();
}

void GameBox::Render()
{
    SetRender();
    mesh->Draw();
}

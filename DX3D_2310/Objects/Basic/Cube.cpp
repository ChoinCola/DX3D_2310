#include "Framework.h"

Cube::Cube(Float3 size, Vector3 pos)
    : size(size)
{
    mesh = new Mesh<VertexType>();

    SetLocalPosition(pos);
    MakeMesh();
    MakeNormal();
    MakeTangent();
    mesh->CreateMesh();
}

Cube::~Cube()
{
    delete mesh;
}

void Cube::Render()
{
    SetRender();
    mesh->Draw();
}

void Cube::MakeMesh()
{
    Float3 halfSize(size.x * 0.5, size.y * 0.5, size.z * 0.5);

    vector<VertexType>& vertices = mesh->GetVertices();

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
    vertices.emplace_back(+halfSize.x, +halfSize.y, -halfSize.z, 0, 0);
    vertices.emplace_back(+halfSize.x, +halfSize.y, +halfSize.z, 1, 0);
    vertices.emplace_back(+halfSize.x, -halfSize.y, -halfSize.z, 0, 1);
    vertices.emplace_back(+halfSize.x, -halfSize.y, +halfSize.z, 1, 1);

    //Left
    vertices.emplace_back(-halfSize.x, +halfSize.y, -halfSize.z, 0, 0);
    vertices.emplace_back(-halfSize.x, -halfSize.y, -halfSize.z, 0, 1);
    vertices.emplace_back(-halfSize.x, +halfSize.y, +halfSize.z, 1, 0);
    vertices.emplace_back(-halfSize.x, -halfSize.y, +halfSize.z, 1, 1);

    //Back
    vertices.emplace_back(-halfSize.x, +halfSize.y, +halfSize.z, 0, 0);
    vertices.emplace_back(-halfSize.x, -halfSize.y, +halfSize.z, 0, 1);
    vertices.emplace_back(+halfSize.x, +halfSize.y, +halfSize.z, 1, 0);
    vertices.emplace_back(+halfSize.x, -halfSize.y, +halfSize.z, 1, 1);

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

}

void Cube::MakeTangent()
{
    vector<VertexType>& vertices = mesh->GetVertices();
    vector<UINT>& indices = mesh->GetIndices();

    FOR(indices.size() / 3)
    {
        UINT index0 = indices[i * 3 + 0];
        UINT index1 = indices[i * 3 + 1];
        UINT index2 = indices[i * 3 + 2];

        Vector3 p0 = vertices[index0].pos;
        Vector3 p1 = vertices[index1].pos;
        Vector3 p2 = vertices[index2].pos;

        Float2 uv0 = vertices[index0].uv;
        Float2 uv1 = vertices[index1].uv;
        Float2 uv2 = vertices[index2].uv;

        Vector3 e0 = p1 - p0;
        Vector3 e1 = p2 - p0;

        float u0 = uv1.x - uv0.x;
        float v0 = uv1.y - uv0.y;
        float u1 = uv2.x - uv0.x;
        float v1 = uv2.y - uv0.y;

        float d = 1.0f / (u0 * v1 - v0 * u1);
        Vector3 tangent = d * (e0 * v1 - e1 * v0);

        vertices[index0].tangent += tangent;
        vertices[index1].tangent += tangent;
        vertices[index2].tangent += tangent;
    }
}
void Cube::MakeNormal()
{
    vector<VertexType>& vertices = mesh->GetVertices();
    vector<UINT>& indices = mesh->GetIndices();

    FOR(indices.size() / 3)
    {
        UINT index0 = indices[i * 3 + 0];
        UINT index1 = indices[i * 3 + 1];
        UINT index2 = indices[i * 3 + 2];

        Vector3 v0 = vertices[index0].pos;
        Vector3 v1 = vertices[index1].pos;
        Vector3 v2 = vertices[index2].pos;

        Vector3 e0 = v1 - v0;
        Vector3 e1 = v2 - v0;

        Vector3 noraml = Vector3::Cross(e0, e1).GetNormalized();

        vertices[index0].normal += noraml;
        vertices[index1].normal += noraml;
        vertices[index2].normal += noraml;
    }
}

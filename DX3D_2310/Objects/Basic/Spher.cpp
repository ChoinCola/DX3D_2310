#include "Framework.h"

Spher::Spher(float size, UINT dividecount)
{
    material = new Material(L"VertexColorShader.hlsl", VERTEX_COLOR);
    mesh = new Mesh<VertexColor>();

    // �� 20��ü�� �� �ﰢ�� ���� Ȳ�ݺ�.
    double X = 0.525731f;
    double Z = 0.850651f;
    vector<VertexColor>& vertices = mesh->GetVertices();
    vertices.emplace_back(-X, 0.0f, +Z, 0, 1, 1);
    vertices.emplace_back(+X, 0.0f, +Z, 1, 1, 1);
    vertices.emplace_back(-X, 0.0f, -Z, 0, 0, 1);
    vertices.emplace_back(+X, 0.0f, -Z, 1, 0, 1);

    vertices.emplace_back(0.0f, +Z, +X, 0, 1, 1);
    vertices.emplace_back(0.0f, +Z, -X, 1, 1, 1);
    vertices.emplace_back(0.0f, -Z, +X, 0, 0, 1);
    vertices.emplace_back(0.0f, -Z, -X, 1, 0, 1);

    vertices.emplace_back(+Z, +X, 0.0f, 0, 1, 1);
    vertices.emplace_back(-Z, +X, 0.0f, 1, 1, 1);
    vertices.emplace_back(+Z, -X, 0.0f, 0, 0, 1);
    vertices.emplace_back(-Z, -X, 0.0f, 1, 0, 1);

    vector<UINT>& indices = mesh->GetIndices();
     //�� �������� �ﰢ�� ���·� �̾ �ߴ� ������ �ð�������� ��ġ.
    indices =
    {
       1,4,0,
       4,9,0,
       4,5,9,
       8,5,4,

       1,8,4,
       1,10,8,
       10,3,8,
       8,3,5,

       3,2,5,
       3,7,2,
       3,10,7,
       10,6,7,

       6,11,7,
       6,0,11,
       6,1,0,
       10,1,6,

       11,0,9,
       2,11,9,
       5,2,9,
       11,2,7
    };

    // ��üȭ �Լ�
    for (UINT i = 0; i < dividecount; i++)
        Subdivide();

    /*
        Subdivide(); �Լ���ζ��
        ���Ҹ��� ���ؽ� ���� 2���� ������.

        1�� = 24��
        2�� = 48��
        3�� = 96��
        4�� = 192��

        6�� �̻����δ� ��������..
    */

    // �� ���ؽ��� ��ġ�� �����̶� ����������� �������.
    for (UINT i = 0; i < vertices.size(); i++) {
        XMVECTOR def = XMVector3Normalize(XMLoadFloat3(&vertices[i].pos));
        XMVECTOR ontoSphere = size * def;
        XMStoreFloat3(&vertices[i].pos, ontoSphere);
    }

    mesh->CreateMesh();

    worldBuffer = new MatrixBuffer();

    ScratchImage image;
    LoadFromWICFile(L"Textures/Landscape/Box.png", WIC_FLAGS_NONE, nullptr, image);
    // srv�Ҵ�
    CreateShaderResourceView(DEVICE, image.GetImages(), image.GetImageCount(), image.GetMetadata(), &srv);
}

Spher::~Spher()
{
    delete mesh;
    delete worldBuffer;
    srv->Release();
}

void Spher::Update()
{
}

void Spher::Render()
{
    worldBuffer->Set(world);
    worldBuffer->SetVS(0);
    DC->PSSetShaderResources(0, 1, &srv);
    material->Set();

    mesh->Draw();
}

VertexColor Spher::MidPoint(const VertexColor& v0, const VertexColor& v1)
{
    XMVECTOR p0 = XMLoadFloat3(&v0.pos);
    XMVECTOR p1 = XMLoadFloat3(&v1.pos);

    XMVECTOR c0 = XMLoadFloat4(&v0.color);
    XMVECTOR c1 = XMLoadFloat4(&v1.color);

    // Compute the midpoints of all the attributes.  Vectors need to be normalized
    // since linear interpolating can make them not unit length.  
    XMVECTOR pos = 0.5f * (p0 + p1);
    XMVECTOR color = XMVector4Normalize(0.5f * (c0 + c1));

    VertexColor v;
    XMStoreFloat3(&v.pos, pos);
    XMStoreFloat4(&v.color, color);

    return v;
}

void Spher::Subdivide()
{
    // Save a copy of the input geometry.
    vector<VertexColor> verticesCopy;
    vector<VertexColor>& nowVertex = mesh->GetVertices();

    for (const auto& def : nowVertex)
        verticesCopy.push_back(def);

    vector<UINT> indicesCopy;
    vector<UINT>& nowIndices = mesh->GetIndices();

    for (auto def : nowIndices)
        indicesCopy.push_back(def);

    nowVertex.resize(0);
    nowIndices.resize(0);

    //       v1
    //       *
    //      / \
    //     /   \
    //  m0*-----*m1
    //   / \   / \
    //  /   \ /   \
    // *-----*-----*
    // v0    m2     v2

    UINT numTris = indicesCopy.size() / 3;
    for (UINT i = 0; i < numTris; ++i)
    {
        VertexColor v0 = verticesCopy[indicesCopy[i * 3 + 0]];
        VertexColor v1 = verticesCopy[indicesCopy[i * 3 + 1]];
        VertexColor v2 = verticesCopy[indicesCopy[i * 3 + 2]];

        //
        // Generate the midpoints.
        //

        VertexColor m0 = MidPoint(v0, v1);
        VertexColor m1 = MidPoint(v1, v2);
        VertexColor m2 = MidPoint(v0, v2);

        //
        // Add new geometry.
        //

        nowVertex.push_back(v0); // 0
        nowVertex.push_back(v1); // 1
        nowVertex.push_back(v2); // 2
        nowVertex.push_back(m0); // 3
        nowVertex.push_back(m1); // 4
        nowVertex.push_back(m2); // 5

        nowIndices.push_back(i * 6 + 0);
        nowIndices.push_back(i * 6 + 3);
        nowIndices.push_back(i * 6 + 5);

        nowIndices.push_back(i * 6 + 3);
        nowIndices.push_back(i * 6 + 4);
        nowIndices.push_back(i * 6 + 5);

        nowIndices.push_back(i * 6 + 5);
        nowIndices.push_back(i * 6 + 4);
        nowIndices.push_back(i * 6 + 2);

        nowIndices.push_back(i * 6 + 3);
        nowIndices.push_back(i * 6 + 1);
        nowIndices.push_back(i * 6 + 4);
    }
}

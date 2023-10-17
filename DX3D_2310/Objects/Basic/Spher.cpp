#include "Framework.h"

Spher::Spher(float size, UINT dividecount)
{
    material = new Material(L"VertexColorShader.hlsl");
    mesh = new Mesh<VertexColor>();

    // 정 20면체의 각 삼각형 길이 황금비.
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
     //각 꼭짓점을 삼각형 형태로 이어서 긋는 순서를 시계방향으로 배치.
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

    // 구체화 함수
    for (UINT i = 0; i < dividecount; i++)
        Subdivide();

    /*
        Subdivide(); 함수대로라면
        분할마다 버텍스 수가 2곱씩 증가함.

        1번 = 24개
        2번 = 48개
        3번 = 96개
        4번 = 192개

        6번 이상으로는 하지말기..
    */

    // 각 버텍스의 위치를 원점이랑 절대기준으로 띄워놓음.
    for (UINT i = 0; i < vertices.size(); i++) {
        XMVECTOR def = XMVector3Normalize(XMLoadFloat3(&vertices[i].pos));
        XMVECTOR ontoSphere = size * def;
        XMStoreFloat3(&vertices[i].pos, ontoSphere);
    }

    mesh->CreateMesh();

    worldBuffer = new MatrixBuffer();

    ScratchImage image;
    LoadFromWICFile(L"Textures/Landscape/Box.png", WIC_FLAGS_NONE, nullptr, image);
    // srv할당
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

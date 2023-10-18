#include "Framework.h"

Ground::Ground(UINT Size, UINT BlockCount)
{
    material = new Material(L"Tutorial.hlsl");

    mesh = new Mesh<VertexUV>();
    Backupmesh = new Mesh<VertexUV>();

    float MaxSize = (float)Size / 2;
    float BlockSize = (float)Size / BlockCount;
    
    vector<VertexUV>& vertices = mesh->GetVertices();
    for (int i = 0; i <= BlockCount; i++) {
        for (int j = 0; j <= BlockCount; j++) {
            vertices.emplace_back(
                //Pos
                  - MaxSize + j * BlockSize
                , 0
                , + MaxSize - i * BlockSize
                //UV
                , 1 / BlockCount * j
                , 1 / BlockCount * i);
        }
    }

    vector<UINT>& indices = mesh->GetIndices();
    indices.reserve(pow(BlockCount, 2) * 6);
    for (int i = 0; i < pow(BlockCount,2); i++) {
        indices.emplace_back(i + BlockCount + 1 + (i / BlockCount));
        indices.emplace_back(i + (i / BlockCount));
        indices.emplace_back(i + BlockCount + 2 + (i / BlockCount));
        indices.emplace_back(i + BlockCount + 2 + (i / BlockCount));
        indices.emplace_back(i + (i / BlockCount));
        indices.emplace_back(i + 1 + (i / BlockCount));
    }
    /*
        1        3
        ----------
        1        1
        1        1
        1        1
        ----------
        0        2
    */

    

    // 데이터 기입. 백업본을 따로 넣는다.
    vector<VertexUV>& Backupvertices = Backupmesh->GetVertices();
    vector<UINT>& Backupindices = Backupmesh->GetIndices();
    // 데이터 사이즈 미리 지정
    Backupvertices.reserve(vertices.size());
    Backupindices.reserve(indices.size());
    // 깊은복사 실행
    for (VertexUV vertice : vertices)
        Backupvertices.emplace_back(vertice);

    for (UINT vertice : indices)
        Backupindices.emplace_back(vertice);
    // 기본메시와 백업메시를 생성

    mesh->CreateMesh();
    Backupmesh->CreateMesh();

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

void Ground::DivideUpdate(UINT inputNumber)
{
    // 새롭게 시전하는 DivideUpdate가 기존 Divide와 다를경우,
    if (nowDivideNumber != inputNumber) {
        // 기본mesh를 백업본mesh로 초기화
        vector<VertexUV>& Backupvertices = Backupmesh->GetVertices();
        vector<UINT>& Backupindices = Backupmesh->GetIndices();

        vector<VertexUV>& vertices = mesh->GetVertices();
        vector<UINT>& indices = mesh->GetIndices();

        // 초기화 후 리사이징
        vertices.clear();
        indices.clear();
        vertices.reserve(Backupvertices.size());
        indices.reserve(Backupindices.size());

        for (VertexUV def : Backupvertices)
            vertices.emplace_back(def);

        for (UINT def : Backupindices)
            indices.emplace_back(def);

        // 다시 자르기.
        for (int i = 0; i < inputNumber; i++)
            Subdivide();

        mesh->CreateMesh();
        // 종료 후 현재 Divide 횟수를 저장.
        nowDivideNumber = inputNumber;
    }
}

void Ground::Render()
{
    worldBuffer->Set(world);
    worldBuffer->SetVS(0);

    material->Set();

    mesh->Draw();
}

VertexUV Ground::MidPoint(const VertexUV& v0, const VertexUV& v1)
{
    XMVECTOR p0 = XMLoadFloat3(&v0.pos);
    XMVECTOR p1 = XMLoadFloat3(&v1.pos);

    XMVECTOR c0 = XMLoadFloat2(&v0.uv);
    XMVECTOR c1 = XMLoadFloat2(&v1.uv);

    // Compute the midpoints of all the attributes.  Vectors need to be normalized
    // since linear interpolating can make them not unit length.  
    XMVECTOR pos = 0.5f * (p0 + p1);
    XMVECTOR uv = 0.5f * (c0 + c1);

    VertexUV v;
    XMStoreFloat3(&v.pos, pos);
    XMStoreFloat2(&v.uv, uv);

    return v;
}

void Ground::Subdivide()
{
    // Save a copy of the input geometry.
    vector<VertexUV> verticesCopy;
    vector<VertexUV>& nowVertex = mesh->GetVertices();
    verticesCopy.reserve(nowVertex.size());

    for (const auto& def : nowVertex)
        verticesCopy.push_back(def);

    vector<UINT> indicesCopy;
    vector<UINT>& nowIndices = mesh->GetIndices();
    indicesCopy.reserve(nowIndices.size());

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
        VertexUV v0 = verticesCopy[indicesCopy[i * 3 + 0]];
        VertexUV v1 = verticesCopy[indicesCopy[i * 3 + 1]];
        VertexUV v2 = verticesCopy[indicesCopy[i * 3 + 2]];

        //
        // Generate the midpoints.
        //

        VertexUV m0 = MidPoint(v0, v1);
        VertexUV m1 = MidPoint(v1, v2);
        VertexUV m2 = MidPoint(v0, v2);

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

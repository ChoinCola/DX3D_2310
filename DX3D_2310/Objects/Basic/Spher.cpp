#include "Framework.h"

Spher::Spher(float size, UINT dividecount)
{
    material->SetShader(L"Light/DiffuseLight.hlsl");
    material->SetDiffuseMap(L"Textures/Colors/White.png");
    RSset = new RasterizerState();
    mesh = new Mesh<VertexType>();
    BackUpmesh = new Mesh<VertexType>();
    RSset->SetState();
    this->size = size;
    MakeMesh(size, dividecount);
    MakeNormal();
    BackUpMesh();

    mesh->CreateMesh();
    BackUpmesh->CreateMesh();
}

Spher::~Spher()
{
    delete mesh;
}

void Spher::Update()
{
    if (dividecountold != dividecountnow)
    {
        delete mesh;
        mesh = new Mesh<VertexType>();

        vector<VertexType>& vertices = mesh->GetVertices();
        vector<UINT>& indices = mesh->GetIndices();

        vector<VertexType>& Bvertices = BackUpmesh->GetVertices();
        vector<UINT>& Bindices = BackUpmesh->GetIndices();
        
        vertices.reserve(Bvertices.size());
        indices.reserve(Bindices.size());


        for (VertexType& def : Bvertices)
            vertices.emplace_back(def);

        for (UINT& def : Bindices)
            indices.emplace_back(def);

        for (int i = 0; i < dividecountnow; i++)
            Subdivide();

        for (UINT i = 0; i < vertices.size(); i++) {
            XMVECTOR def = XMVector3Normalize(XMLoadFloat3(&vertices[i].pos));
            XMVECTOR ontoSphere = size * def;
            XMStoreFloat3(&vertices[i].pos, ontoSphere);
        }

        MakeNormal();
        mesh->CreateMesh();
        dividecountold = dividecountnow;
    }
}

void Spher::Render()
{
    if (Setline)
    {
        RSset->CullMode(D3D11_CULL_NONE);
        RSset->FillMode(D3D11_FILL_WIREFRAME);
        RSset->SetState();
    }
    else if (Setline)
    {
        RSset->CullMode(D3D11_CULL_BACK);
        RSset->FillMode(D3D11_FILL_SOLID);
        RSset->SetState();
    }

    SetRender();
    mesh->Draw();
}

void Spher::GUIRender()
{
    if (ImGui::Button("_Set_UVMap"))
        DIALOG->OpenDialog("Save", "Save", ".png, .jpg, .tga", ".");

    if (DIALOG->Display("Save"))
    {
        if (DIALOG->IsOk()) {
            string file = DIALOG->GetFilePathName();
            material->SetDiffuseMap(ToWString(file));
            DIALOG->Close();
        }
        else
            DIALOG->Close();
    }
    //ImGui::Checkbox("Terrain_Onley_Line", &Setline);
    if (ImGui::Button("View_Line"))
        Setline = 1;
    if (ImGui::Button("View_Race"))
        Setline = 0;

    string temp = tag + "_Subdivide_Count";
    ImGui::SliderInt(temp.c_str(), (int*)&dividecountnow, 1,7);
    __super::GUIRender();
}

VertexUVNormal Spher::MidPoint(const VertexType& v0, const VertexType& v1)
{
    XMVECTOR p0 = XMLoadFloat3(&v0.pos);
    XMVECTOR p1 = XMLoadFloat3(&v1.pos);

    XMVECTOR c0 = XMLoadFloat2(&v0.uv);
    XMVECTOR c1 = XMLoadFloat2(&v1.uv);

    XMVECTOR e0 = XMLoadFloat3(&v0.normal);
    XMVECTOR e1 = XMLoadFloat3(&v1.normal);

    // Compute the midpoints of all the attributes.  Vectors need to be normalized
    // since linear interpolating can make them not unit length.  
    XMVECTOR pos = 0.5f * (p0 + p1);
    XMVECTOR uv = XMVector2Normalize(0.5f * (c0 + c1));
    XMVECTOR normal = XMVector3Normalize(e0 + e1);

    VertexType v;
    XMStoreFloat3(&v.pos, pos);
    XMStoreFloat2(&v.uv, uv);
    XMStoreFloat3(&v.normal, normal);

    return v;
}

void Spher::BackUpMesh()
{
    vector<VertexType>& vertices = mesh->GetVertices();
    vector<UINT>& indices = mesh->GetIndices();

    vector<VertexType>& Bvertices = BackUpmesh->GetVertices();
    vector<UINT>& Bindices = BackUpmesh->GetIndices();

    Bvertices.reserve(vertices.size());
    Bindices.reserve(indices.size());

    for (VertexType& def : vertices)
        Bvertices.push_back(def);

    for (UINT& def : indices)
        Bindices.push_back(def);
}

void Spher::MakeMesh(float size, UINT dividecount)
{
    // 정 20면체의 각 삼각형 길이 황금비.
    double X = 0.525731f;
    double Z = 0.850651f;
    vector<VertexType>& vertices = mesh->GetVertices();
    vertices.emplace_back(-X, 0.0f, +Z, 0, 1);
    vertices.emplace_back(+X, 0.0f, +Z, 1, 1);
    vertices.emplace_back(-X, 0.0f, -Z, 0, 0);
    vertices.emplace_back(+X, 0.0f, -Z, 1, 0);

    vertices.emplace_back(0.0f, +Z, +X, 0, 1);
    vertices.emplace_back(0.0f, +Z, -X, 1, 1);
    vertices.emplace_back(0.0f, -Z, +X, 0, 0);
    vertices.emplace_back(0.0f, -Z, -X, 1, 0);

    vertices.emplace_back(+Z, +X, 0.0f, 0, 1);
    vertices.emplace_back(-Z, +X, 0.0f, 1, 1);
    vertices.emplace_back(+Z, -X, 0.0f, 0, 0);
    vertices.emplace_back(-Z, -X, 0.0f, 1, 0);

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
}

void Spher::Subdivide()
{
    // Save a copy of the input geometry.
    vector<VertexType> verticesCopy;
    vector<VertexType>& nowVertex = mesh->GetVertices();

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
        VertexType v0 = verticesCopy[indicesCopy[i * 3 + 0]];
        VertexType v1 = verticesCopy[indicesCopy[i * 3 + 1]];
        VertexType v2 = verticesCopy[indicesCopy[i * 3 + 2]];

        //
        // Generate the midpoints.
        //

        VertexType m0 = MidPoint(v0, v1);
        VertexType m1 = MidPoint(v1, v2);
        VertexType m2 = MidPoint(v0, v2);

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

void Spher::MakeNormal()
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

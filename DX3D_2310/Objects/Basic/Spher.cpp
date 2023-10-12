#include "Framework.h"

Spher::Spher(float size, UINT dividecount)
{
    // �� 20��ü�� �� �ﰢ�� ���� Ȳ�ݺ�.
    double X = 0.525731f;
    double Z = 0.850651f;

    vertices.emplace_back(-X, 0.0f, +Z, 0, 1, 0);
    vertices.emplace_back(+X, 0.0f, +Z, 0, 1, 0);
    vertices.emplace_back(-X, 0.0f, -Z, 0, 1, 0);
    vertices.emplace_back(+X, 0.0f, -Z, 0, 1, 0);

    vertices.emplace_back(0.0f, +Z, +X, 1, 0, 0);
    vertices.emplace_back(0.0f, +Z, -X, 1, 0, 0);
    vertices.emplace_back(0.0f, -Z, +X, 1, 0, 0);
    vertices.emplace_back(0.0f, -Z, -X, 1, 0, 0);

    vertices.emplace_back(+Z, +X, 0.0f, 0, 0, 1);
    vertices.emplace_back(-Z, +X, 0.0f, 0, 0, 1);
    vertices.emplace_back(+Z, -X, 0.0f, 0, 0, 1);
    vertices.emplace_back(-Z, -X, 0.0f, 0, 0, 1);

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

    vertexShader = new VertexShader(L"Shaders/Tutorial.hlsl");
    pixelShader = new PixelShader(L"Shaders/Tutorial.hlsl");

    vertexBuffer = new VertexBuffer(vertices.data(), sizeof(VertexColor), vertices.size());
    indexBuffer = new IndexBuffer(indices.data(), indices.size());

    worldBuffer = new MatrixBuffer();
}

Spher::~Spher()
{
    delete vertexShader;
    delete pixelShader;
    delete vertexBuffer;
    delete indexBuffer;
    delete worldBuffer;
}

void Spher::Update()
{
    //if (KEYBOARD->Press('W'))
    //    matworld._43 += DELTA;
    //if (KEYBOARD->Press('S'))
    //    matworld._43 -= DELTA;
    //if (KEYBOARD->Press('A'))
    //    matworld._41 -= DELTA;
    //if (KEYBOARD->Press('D'))
    //    matworld._41 += DELTA;
    //if (KEYBOARD->Press('Q'))
    //    matworld._42 += DELTA;
    //if (KEYBOARD->Press('E'))
    //    matworld._42 -= DELTA;

    //if (KEYBOARD->Press('T'))
    //    matworld._33 += DELTA;
    //if (KEYBOARD->Press('G'))
    //    matworld._33 -= DELTA;
    //if (KEYBOARD->Press('F'))
    //    matworld._11 -= DELTA;
    //if (KEYBOARD->Press('H'))
    //    matworld._11 += DELTA;
    //if (KEYBOARD->Press('R'))
    //    matworld._22 += DELTA;
    //if (KEYBOARD->Press('Y'))
    //    matworld._22 -= DELTA;


    //static float angle = 0.0f;
    //if (KEYBOARD->Press(VK_UP))
    //    angle += DELTA;
    //if (KEYBOARD->Press(VK_DOWN))
    //    angle -= DELTA;

    //matworld._22 = cos(angle);
    //matworld._23 = sin(angle);
    //matworld._32 = -sin(angle);
    //matworld._33 = cos(angle);


    //world = XMLoadFloat4x4(&matworld);
    //worldBuffer->Set(world);
}

void Spher::Render()
{
    worldBuffer->Set(world);
    worldBuffer->SetVS(0);

    vertexBuffer->Set(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    indexBuffer->Set();

    vertexShader->Set();
    pixelShader->Set();

    DC->DrawIndexed(indices.size(), 0, 0);
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
    for (auto def : vertices)
        verticesCopy.push_back(def);

    vector<UINT> indicesCopy;
    for (auto def : indices)
        indicesCopy.push_back(def);

    vertices.resize(0);
    indices.resize(0);

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

        vertices.push_back(v0); // 0
        vertices.push_back(v1); // 1
        vertices.push_back(v2); // 2
        vertices.push_back(m0); // 3
        vertices.push_back(m1); // 4
        vertices.push_back(m2); // 5

        indices.push_back(i * 6 + 0);
        indices.push_back(i * 6 + 3);
        indices.push_back(i * 6 + 5);

        indices.push_back(i * 6 + 3);
        indices.push_back(i * 6 + 4);
        indices.push_back(i * 6 + 5);

        indices.push_back(i * 6 + 5);
        indices.push_back(i * 6 + 4);
        indices.push_back(i * 6 + 2);

        indices.push_back(i * 6 + 3);
        indices.push_back(i * 6 + 1);
        indices.push_back(i * 6 + 4);
    }
}

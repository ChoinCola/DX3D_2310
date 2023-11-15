#include "Framework.h"

Model::Model(string name) : name(name)
{
	SetTag(name);

	ReadMaterial();
	ReadMesh();

	worldBuffer = new MatrixBuffer();
}

Model::~Model()
{
	for (Material* material : materials)
		delete material;

	for (ModelMesh* mesh : meshes)
		delete mesh;

	delete worldBuffer;
}

void Model::Render()
{
	worldBuffer->Set(world);
	worldBuffer->SetVS(0);

	for (ModelMesh* mesh : meshes)
		mesh->Render();
}

void Model::GUIRender()
{
	Transform::GUIRender();

	for (Material* material : materials)
		material->GUIRender();
}

void Model::SetShader(wstring file)
{
	for (Material* material : materials)
		material->SetShader(file);
}

void Model::ReadMaterial()
{
    string file = "Models/Materials/" + name + "/" + name + ".mats";
    // Material ������ ������ ��θ� �����մϴ�. �� ��δ� Material ������ ����� ���͸��� ���� �̸����� �����˴ϴ�.

    BinaryReader* reader = new BinaryReader(file);
    // Material ������ ������ �б� ���� BinaryReader�� �����ϰ� ������ ���ϴ�.

    if (reader->IsFailed()) {

        ModelExporter* exporter;

        file = "Models/FBX/" + name + ".fbx";

        exporter = new ModelExporter(name, file);
        exporter->ExportMaterial();
        exporter->ExportMesh();

        file = "Models/Materials/" + name + "/" + name + ".mats";
        // Material ������ ������ ��θ� �����մϴ�. �� ��δ� Material ������ ����� ���͸��� ���� �̸����� �����˴ϴ�.

        reader = new BinaryReader(file);
        // Material ������ ������ �б� ���� BinaryReader�� �����ϰ� ������ ���ϴ�.
        delete exporter;
        // �ٽúҷ����⵵ �����ϸ� �׳� ��Ʈ��.
        if (reader->IsFailed())
            assert(false);

    }
    // ���� ���⿡ ������ ���, assert �Լ��� ����Ͽ� ���α׷� ������ �ߴ��մϴ�.

    UINT size = reader->UInt();
    // Material �������� ������ ���Ͽ��� �о�ɴϴ�.

    materials.reserve(size);
    // Material �����͸� ������ materials ���͸� ���� �����մϴ�.

    FOR(size)
    {
        Material* material = new Material();
        // Material Ŭ������ �ν��Ͻ��� �����մϴ�.

        material->Load(reader->String());
        // Material ������ ���Ͽ��� Material ������ �о�ͼ� Material ��ü�� �ε��մϴ�.

        materials.push_back(material);
        // Material ��ü�� materials ���Ϳ� �߰��մϴ�.
    }

    delete reader;
    // ����� ���� BinaryReader�� �����մϴ�.
}

void Model::ReadMesh()
{
    // ����� Mesh ������ ���� ��� ����
    string file = "Models/Meshes/" + name + ".mesh";

    // BinaryReader�� ����Ͽ� Mesh ������ ������ ���� �б� ���� ������ ���ϴ�.
    BinaryReader* reader = new BinaryReader(file);

    // ���� ���⿡ ������ ���, assert �Լ��� ����Ͽ� ������ ó���մϴ�.
    // assert�ϴ°� �� ã�� ���� �����ϱ� ����,
    if (reader->IsFailed()) assert(false);

    // Mesh �������� ������ ���Ͽ��� �о�ɴϴ�.
    // ������ ������ �˾ƾ� ����������.
    UINT size = reader->UInt();
    // ������ ���Ե� ������� �о���Ѵ�.
  
    // Mesh �����͸� ������ meshes ���͸� ���� �����մϴ�.
    meshes.reserve(size);

    // Mesh ������ ������ŭ �ݺ��Ͽ� Mesh �����͸� �о�ɴϴ�.
    FOR(size)
    {
        // Mesh�� �̸��� ���Ͽ��� �о�ɴϴ�.
        string meshName = reader->String();

        // ModelMesh Ŭ������ �ν��Ͻ��� �����ϰ� Mesh�� �̸��� �����մϴ�.
        ModelMesh* mesh = new ModelMesh(meshName);

        // Mesh�� �ش��ϴ� Material�� materials ���Ϳ��� ã�� �����մϴ�.
        mesh->SetMaterial(materials[reader->UInt()]);

        // Mesh�� ���� (vertices) ������ ���Ͽ��� �о�ɴϴ�.
        UINT vertexCount = reader->UInt();

        // ���� �����͸� ������ �迭�� �����մϴ�.
        ModelVertex* vertices = new ModelVertex[vertexCount];

        // ���Ͽ��� ���� �����͸� �о�� �迭�� �����մϴ�.
        reader->Byte((void**)&vertices, sizeof(ModelVertex) * vertexCount);

        // Mesh�� �ε��� (indices) ������ ���Ͽ��� �о�ɴϴ�.
        UINT indexCount = reader->UInt();

        // �ε��� �����͸� ������ �迭�� �����մϴ�.
        UINT* indices = new UINT[indexCount];

        // ���Ͽ��� �ε��� �����͸� �о�� �迭�� �����մϴ�.
        reader->Byte((void**)&indices, sizeof(UINT) * indexCount);

        // Mesh�� �����ϰ� ������ �ε��� �����͸� �����մϴ�.
        mesh->CreateMesh(vertices, vertexCount, indices, indexCount);

        // meshes ���Ϳ� Mesh ��ü�� �߰��մϴ�.
        meshes.push_back(mesh);
    }

    size = reader->UInt();
    nodes.resize(size);
    for (NodeData& node : nodes)
    {
        node.index = reader->Int();
        node.name = reader->String();
        node.parent = reader->Int();
        node.transform = reader->Matrix();
    }

    // ����� ���� BinaryReader�� �����մϴ�.
    delete reader;

    MakeBoneTransforms();
}

void Model::MakeBoneTransforms()
{
    boneTransforms.reserve(nodes.size());

    for (NodeData node: nodes)
    {
        Matrix parent;
        int parentIndex = node.parent;

        if (parentIndex < 0)
            parent = XMMatrixIdentity();
        else
            parent = boneTransforms[parentIndex];

        boneTransforms.push_back(node.transform * parent);
        nodeTransforms[node.name] = boneTransforms.back();
    }
}

#include "Framework.h"

Model::Model(string name) : name(name)
{
	SetTag(name);

	ReadMaterial();
	ReadMesh();
    MakeTreenode();
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
    RenderTreenode(0);
}

void Model::SetShader(wstring file)
{
	for (Material* material : materials)
		material->SetShader(file);
}

Material* Model::AddMaterial()
{
    Material* result = new Material();
    materials.push_back(result);

    return result;
}

void Model::ReadMaterial()
{
    string file = "Models/Materials/" + name + "/" + name + ".mats";
    // Material 데이터 파일의 경로를 생성합니다. 이 경로는 Material 파일이 저장된 디렉터리와 파일 이름으로 구성됩니다.

    BinaryReader* reader = new BinaryReader(file);
    // Material 데이터 파일을 읽기 위한 BinaryReader를 생성하고 파일을 엽니다.

    if (reader->IsFailed()) {

        ModelExporter* exporter;

        file = "Models/FBX/" + name + ".fbx";

        exporter = new ModelExporter(name, file);
        exporter->ExportMaterial();

        file = "Models/Materials/" + name + "/" + name + ".mats";
        // Material 데이터 파일의 경로를 생성합니다. 이 경로는 Material 파일이 저장된 디렉터리와 파일 이름으로 구성됩니다.

        reader = new BinaryReader(file);
        // Material 데이터 파일을 읽기 위한 BinaryReader를 생성하고 파일을 엽니다.
        delete exporter;
        // 다시불러오기도 실패하면 그냥 터트림.
        if (reader->IsFailed())
            assert(false);

    }
    // 파일 열기에 실패한 경우, assert 함수를 사용하여 프로그램 실행을 중단합니다.

    UINT size = reader->UInt();
    // Material 데이터의 개수를 파일에서 읽어옵니다.

    materials.reserve(size);
    // Material 데이터를 저장할 materials 벡터를 사전 예약합니다.

    FOR(size)
    {
        Material* material = new Material();
        // Material 클래스의 인스턴스를 생성합니다.

        material->Load(reader->String());
        // Material 데이터 파일에서 Material 정보를 읽어와서 Material 객체에 로드합니다.

        materials.push_back(material);
        // Material 객체를 materials 벡터에 추가합니다.
    }

    delete reader;
    // 사용이 끝난 BinaryReader를 삭제합니다.
}

void Model::ReadMesh()
{
    // 저장된 Mesh 데이터 파일 경로 생성
    string file = "Models/Meshes/" + name + ".mesh";

    // BinaryReader를 사용하여 Mesh 데이터 파일을 열고 읽기 위해 파일을 엽니다.
    BinaryReader* reader = new BinaryReader(file);

    // 파일 열기에 실패한 경우, assert 함수를 사용하여 오류를 처리합니다.
    // assert하는게 더 찾기 쉽고 안전하기 때문,
    if (reader->IsFailed()) {
        ModelExporter* exporter;

        file = "Models/FBX/" + name + ".fbx";

        exporter = new ModelExporter(name, file);
        exporter->ExportMesh();

        file = "Models/Meshes/" + name + ".mesh";
        // Material 데이터 파일의 경로를 생성합니다. 이 경로는 Material 파일이 저장된 디렉터리와 파일 이름으로 구성됩니다.

        reader = new BinaryReader(file);
        // Material 데이터 파일을 읽기 위한 BinaryReader를 생성하고 파일을 엽니다.
        delete exporter;
        // 다시불러오기도 실패하면 그냥 터트림.
        if (reader->IsFailed())
            assert(false);
    }

    // Mesh 데이터의 개수를 파일에서 읽어옵니다.
    // 데이터 개수를 알아야 돌릴수있음.
    UINT size = reader->UInt();
    // 파일은 기입된 순서대로 읽어야한다.
  
    // Mesh 데이터를 저장할 meshes 벡터를 사전 예약합니다.
    meshes.reserve(size);

    // Mesh 데이터 개수만큼 반복하여 Mesh 데이터를 읽어옵니다.
    FOR(size)
    {
        // Mesh의 이름을 파일에서 읽어옵니다.
        string meshName = reader->String();

        // ModelMesh 클래스의 인스턴스를 생성하고 Mesh의 이름을 설정합니다.
        ModelMesh* mesh = new ModelMesh(meshName);

        // Mesh에 해당하는 Material을 materials 벡터에서 찾아 설정합니다.
        mesh->SetMaterial(materials[reader->UInt()]);

        // Mesh의 정점 (vertices) 개수를 파일에서 읽어옵니다.
        UINT vertexCount = reader->UInt();

        // 정점 데이터를 저장할 배열을 생성합니다.
        ModelVertex* vertices = new ModelVertex[vertexCount];

        // 파일에서 정점 데이터를 읽어와 배열에 저장합니다.
        reader->Byte((void**)&vertices, sizeof(ModelVertex) * vertexCount);

        // Mesh의 인덱스 (indices) 개수를 파일에서 읽어옵니다.
        UINT indexCount = reader->UInt();

        // 인덱스 데이터를 저장할 배열을 생성합니다.
        UINT* indices = new UINT[indexCount];

        // 파일에서 인덱스 데이터를 읽어와 배열에 저장합니다.
        reader->Byte((void**)&indices, sizeof(UINT) * indexCount);

        // Mesh를 생성하고 정점과 인덱스 데이터를 설정합니다.
        mesh->CreateMesh(vertices, vertexCount, indices, indexCount);

        // meshes 벡터에 Mesh 객체를 추가합니다.
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

    size = reader->UInt();
    bones.resize(size);
    for (BoneData& bone : bones)
    {
        bone.index = reader->Int();
        bone.name = reader->String();
        bone.offset = reader->Matrix();

        boneMap[bone.name] = bone.index;
    }
    // 사용이 끝난 BinaryReader를 삭제합니다.
    delete reader;

    MakeBoneTransforms();
}

void Model::MakeBoneTransforms()
{
    // 뼈대의 변환 매트릭스를 미리 예약
    boneTransforms.reserve(nodes.size());

    // 각 노드에 대해 반복
    for (NodeData node : nodes)
    {
        Matrix parent;
        int parentIndex = node.parent;

        // 부모 노드가 없으면 단위 행렬로 초기화
        if (parentIndex < 0)
            parent = XMMatrixIdentity();
        else
            parent = boneTransforms[parentIndex];

        // 현재 노드의 변환 매트릭스를 부모의 매트릭스와 곱하여 뼈대의 매트릭스로 저장
        boneTransforms.push_back(node.transform * parent);

        // 노드 이름과 해당 노드의 변환 매트릭스를 맵에 저장
        nodeTransforms[node.name] = boneTransforms.back();
    }
}

void Model::RenderTreenode(int num)
{
    // 트리 노드를 펼치거나 축소하는 버튼 렌더링
    if (ImGui::TreeNode(nodes[num].name.c_str()))
    {
        ImGui::Text(to_string(nodes[num].index).c_str());
        /*
            데이터가 더 필요하다면 nodes[num]에서 데이터를 뽑아서 
            여기에 ImGUI::로 출력하면 됩니다.
        */

        // 자식 노드가 있는 경우 자식 노드를 렌더링
        for (int& child : nodetree[num])
        {
            RenderTreenode(child);
        }
        // 트리 노드를 닫기
        ImGui::TreePop();
    }
}

void Model::MakeTreenode()
{
    nodetree.resize(nodes.size());
    for (auto& node : nodes)
    {
        if (node.index == -1) continue;
        if (node.parent == -1) {
            nodetree[0].emplace_back(node.index);
            continue;
        }
        nodetree[node.parent].emplace_back(node.index);
    }
}
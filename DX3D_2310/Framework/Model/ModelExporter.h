#pragma once

class ModelExporter
{
public:
    // ������: �̸��� ���� ��η� ModelExporter�� �ʱ�ȭ�մϴ�.
    ModelExporter(string name, string file);

    // �Ҹ���: ModelExporter�� �ı��� �� ���ҽ��� �����մϴ�.
    ~ModelExporter();

    // ExportMaterial: ���� ������ �������ϴ�.
    void ExportMaterial();

    // ExportMesh: ���� �޽� �����͸� �������ϴ�.
    void ExportMesh();

    // ExportClip: clipName���� ������ �ִϸ��̼� Ŭ���� �������ϴ�.
    void ExportClip(string clipName);

private:
    // ����(Material) ���� �Լ���

    // ReadMaterial: ������ �о�ɴϴ�.
    void ReadMaterial();

    // WriterMaterial: ���� ������ ���� �����մϴ�.
    void WriterMaterial();

    // CreateTexture: �ؽ�ó ������ �����մϴ�.
    string CreateTexture(string file);

    // �޽�(Mesh) ���� �Լ���

    // ReadBone: �޽��� ����� ���ؽ� ����ġ�� �о�ɴϴ�.
    void ReadBone(aiMesh* mesh, vector<VertexWeights>& vertexWeights);

    // ReadNode: ������ ��� ���� �о�ɴϴ�.
    void ReadNode(aiNode* node, int index, int parent);

    // ReadMesh: �޽ø� �о�ɴϴ�.
    void ReadMesh(aiNode* node);

    // WriteMesh: ���� �޽ø� �����մϴ�.
    void WriteMesh();

    // �ִϸ��̼�(Animation) ���� �Լ���

    // ReadClip: �ִϸ��̼� Ŭ���� �о�ɴϴ�.
    Clip* ReadClip(aiAnimation* animation);

    // ReadKeyFrame: Ŭ���� Ű�������� �о�ɴϴ�.
    void ReadKeyFrame(Clip* clip, aiNode* node, vector<ClipNode>& clopNodes);

    // WriteClip: Ŭ���� �����մϴ�.
    void WriteClip(Clip* clip, string clipName, UINT index);

private:
    // �ִϸ��̼� ���� ���� �Լ���

    // SetClipNode: Ű������ �����Ϳ� ������ ���� �̿��Ͽ� ClipNode�� �����մϴ�.
    void SetClipNode(const KeyData& keyData, const UINT& frameCount, ClipNode& clipNode);

    // CalcInterpolationVector: ���� Ű������ �������� �������� ����մϴ�.
    Float3 CalcInterpolationVector(const vector<KeyVector>& keyData, UINT& count, int curFrame);

    // CalcInterpolationQuat: ���ʹϾ� Ű������ �������� �������� ����մϴ�.
    Float4 CalcInterpolationQuat(const vector<KeyQuat>& keyData, UINT& count, int curFrame);

private:
    Assimp::Importer* importer;
    const aiScene* scene;
    // aiScene�� �ݵ�� ����� ���Ǿ�� �մϴ�.

    string name;

    vector<Material*> materials;
    vector<MeshData*> meshes;
    vector<NodeData*> nodes;
    vector<BoneData*> bones;

    map<string, UINT> boneMap;
    UINT boneCount = 0;
};
#pragma once

class ModelExporter
{
public:
    // 생성자: 이름과 파일 경로로 ModelExporter를 초기화합니다.
    ModelExporter(string name, string file);

    // 소멸자: ModelExporter가 파괴될 때 리소스를 정리합니다.
    ~ModelExporter();

    // ExportMaterial: 모델의 재질을 내보냅니다.
    void ExportMaterial();

    // ExportMesh: 모델의 메시 데이터를 내보냅니다.
    void ExportMesh();

    // ExportClip: clipName으로 지정된 애니메이션 클립을 내보냅니다.
    void ExportClip(string clipName);

private:
    // 재질(Material) 관련 함수들

    // ReadMaterial: 재질을 읽어옵니다.
    void ReadMaterial();

    // WriterMaterial: 읽은 재질을 쓰고 저장합니다.
    void WriterMaterial();

    // CreateTexture: 텍스처 파일을 생성합니다.
    string CreateTexture(string file);

    // 메시(Mesh) 관련 함수들

    // ReadBone: 메시의 뼈대와 버텍스 가중치를 읽어옵니다.
    void ReadBone(aiMesh* mesh, vector<VertexWeights>& vertexWeights);

    // ReadNode: 지정된 노드 값을 읽어옵니다.
    void ReadNode(aiNode* node, int index, int parent);

    // ReadMesh: 메시를 읽어옵니다.
    void ReadMesh(aiNode* node);

    // WriteMesh: 읽은 메시를 저장합니다.
    void WriteMesh();

    // 애니메이션(Animation) 관련 함수들

    // ReadClip: 애니메이션 클립을 읽어옵니다.
    Clip* ReadClip(aiAnimation* animation);

    // ReadKeyFrame: 클립의 키프레임을 읽어옵니다.
    void ReadKeyFrame(Clip* clip, aiNode* node, vector<ClipNode>& clopNodes);

    // WriteClip: 클립을 저장합니다.
    void WriteClip(Clip* clip, string clipName, UINT index);

private:
    // 애니메이션 관련 보조 함수들

    // SetClipNode: 키프레임 데이터와 프레임 수를 이용하여 ClipNode를 설정합니다.
    void SetClipNode(const KeyData& keyData, const UINT& frameCount, ClipNode& clipNode);

    // CalcInterpolationVector: 벡터 키프레임 데이터의 보간값을 계산합니다.
    Float3 CalcInterpolationVector(const vector<KeyVector>& keyData, UINT& count, int curFrame);

    // CalcInterpolationQuat: 쿼터니언 키프레임 데이터의 보간값을 계산합니다.
    Float4 CalcInterpolationQuat(const vector<KeyQuat>& keyData, UINT& count, int curFrame);

private:
    Assimp::Importer* importer;
    const aiScene* scene;
    // aiScene은 반드시 상수로 사용되어야 합니다.

    string name;

    vector<Material*> materials;
    vector<MeshData*> meshes;
    vector<NodeData*> nodes;
    vector<BoneData*> bones;

    map<string, UINT> boneMap;
    UINT boneCount = 0;
};
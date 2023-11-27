#pragma once

class Model : public Transform
{
public:
    // 생성자: 주어진 이름으로 모델 초기화
    Model(string name);

    // 소멸자: 모델이 파괴될 때 자원 정리
    ~Model();

    // 모델을 렌더링하는 함수
    void Render();

    // 모델의 GUI를 렌더링하는 함수
    void GUIRender();

    // 주어진 파일로 Shader를 설정하는 함수
    void SetShader(wstring file);

    Material* AddMaterial();
    void SetMeshMaterial(UINT meshIndex, UINT materialIndex);

    void SaveMateirals();
    void SaveMeshes();

    ModelMesh* GetMesh(UINT index) { return meshes[index]; }
    UINT GetMeshNum() { return meshes.size(); }
    UINT GetMaterialNum() { return materials.size(); }
    Matrix GetNodeTransform(string name) { return nodeTransforms[name]; }
    string GetName() { return name; }

    // 주어진 노드의 로컬 위치를 반환하는 함수
    Vector3 GetNodeLocalPosition(string name)
    {
        Vector3 scale, rotation, translation;
        XMMATRIX matrix = nodeTransforms[name];
        XMMatrixDecompose(scale.GetValue(), rotation.GetValue(), translation.GetValue(), matrix);
        return translation;
    }
    UINT GetBoneIndex(string bonesname) { return boneMap[bonesname]; }
private:
    // 재료 정보를 읽어오는 함수
    void ReadMaterial();

    // 메시 정보를 읽어오는 함수
    void ReadMesh();

    // 뼈대의 변환 매트릭스를 생성하는 함수
    void MakeBoneTransforms();

    // 특정 번호의 트리 노드를 렌더링하는 함수
    void RenderTreenode(int num);

    // 트리 노드를 생성하는 함수
    void MakeTreenode();

    void MeshSetting();

protected:
    string name;                        // 모델의 이름
    vector<Material*> materials;        // 재료 정보를 저장하는 벡터
    vector<ModelMesh*> meshes;          // 모델 메시 정보를 저장하는 벡터
    vector<NodeData> nodes;             // 모델의 노드 정보를 저장하는 벡터
    vector<BoneData> bones;             // 뼈대 정보를 저장하는 벡터

    vector<vector<int>> nodetree;       // 노드의 부모 관계를 저장하는 2D 벡터

    MatrixBuffer* worldBuffer;          // 모델과 메시의 정보를 담는 버퍼

    vector<Matrix> boneTransforms;      // 뼈대의 변환 매트릭스를 저장하는 벡터
    map<string, Matrix> nodeTransforms; // 노드의 변환 매트릭스를 저장하는 맵
    map<string, UINT> boneMap;          // 뼈대의 인덱스를 저장하는 맵

    int selectMeshNum = 0;
    int selectMatarialNum = 0;
};
#pragma once

class Model : public Transform
{
public:
    // ������: �־��� �̸����� �� �ʱ�ȭ
    Model(string name);

    // �Ҹ���: ���� �ı��� �� �ڿ� ����
    ~Model();

    // ���� �������ϴ� �Լ�
    void Render();

    // ���� GUI�� �������ϴ� �Լ�
    void GUIRender();

    // �־��� ���Ϸ� Shader�� �����ϴ� �Լ�
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

    // �־��� ����� ���� ��ġ�� ��ȯ�ϴ� �Լ�
    Vector3 GetNodeLocalPosition(string name)
    {
        Vector3 scale, rotation, translation;
        XMMATRIX matrix = nodeTransforms[name];
        XMMatrixDecompose(scale.GetValue(), rotation.GetValue(), translation.GetValue(), matrix);
        return translation;
    }
    UINT GetBoneIndex(string bonesname) { return boneMap[bonesname]; }
private:
    // ��� ������ �о���� �Լ�
    void ReadMaterial();

    // �޽� ������ �о���� �Լ�
    void ReadMesh();

    // ������ ��ȯ ��Ʈ������ �����ϴ� �Լ�
    void MakeBoneTransforms();

    // Ư�� ��ȣ�� Ʈ�� ��带 �������ϴ� �Լ�
    void RenderTreenode(int num);

    // Ʈ�� ��带 �����ϴ� �Լ�
    void MakeTreenode();

    void MeshSetting();

protected:
    string name;                        // ���� �̸�
    vector<Material*> materials;        // ��� ������ �����ϴ� ����
    vector<ModelMesh*> meshes;          // �� �޽� ������ �����ϴ� ����
    vector<NodeData> nodes;             // ���� ��� ������ �����ϴ� ����
    vector<BoneData> bones;             // ���� ������ �����ϴ� ����

    vector<vector<int>> nodetree;       // ����� �θ� ���踦 �����ϴ� 2D ����

    MatrixBuffer* worldBuffer;          // �𵨰� �޽��� ������ ��� ����

    vector<Matrix> boneTransforms;      // ������ ��ȯ ��Ʈ������ �����ϴ� ����
    map<string, Matrix> nodeTransforms; // ����� ��ȯ ��Ʈ������ �����ϴ� ��
    map<string, UINT> boneMap;          // ������ �ε����� �����ϴ� ��

    int selectMeshNum = 0;
    int selectMatarialNum = 0;
};
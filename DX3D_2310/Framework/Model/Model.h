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

    // �־��� ����� ��ȯ ��Ʈ������ ��ȯ�ϴ� �Լ�
    Matrix GetNodeTransform(string name) { return nodeTransforms[name]; }

    // �־��� ����� ���� ��ġ�� ��ȯ�ϴ� �Լ�
    Vector3 GetNodeLocalPosition(string name)
    {
        Vector3 scale, rotation, translation;
        XMMATRIX matrix = nodeTransforms[name];
        XMMatrixDecompose(scale.GetValue(), rotation.GetValue(), translation.GetValue(), matrix);
        return translation;
    }

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
};
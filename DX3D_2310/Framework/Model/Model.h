#pragma once

class Model : public Transform
{
public:
	Model(string name);
	~Model();

	void Render();
	void GUIRender();

	void SetShader(wstring file);

	Matrix GetNodeTransform(string name) { return nodeTransforms[name]; }
	Vector3 GetNodeLocalPosition(string name)
	{
		Vector3 scale, rotation, translation;
		XMMATRIX matrix = nodeTransforms[name];
		XMMatrixDecompose(scale.GetValue(), rotation.GetValue(), translation.GetValue(), matrix);
		return translation;
	}

private:
	void ReadMaterial();
	void ReadMesh();

	void MakeBoneTransforms();

	void RenderTreenode(int num);
	void MakeTreenode();

protected:
	string name;
	vector<Material*> materials;
	vector<ModelMesh*> meshes;
	vector<NodeData> nodes;

	// ��� �θ���踦 �����ؾ��ϱ� ������ vector<vector> ���·� �����Ѵ�.
	vector<vector<int>> nodetree;

	// �𵨰� �޽��� ������ ������������ �ѹ��� worldBuffer�� �����Ѵ�.
	MatrixBuffer* worldBuffer;

	vector<Matrix> boneTransforms;
	map<string, Matrix> nodeTransforms;
};
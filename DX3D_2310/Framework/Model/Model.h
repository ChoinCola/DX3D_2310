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

	// 노드 부모관계를 정리해야하기 때문에 vector<vector> 형태로 정리한다.
	vector<vector<int>> nodetree;

	// 모델과 메시의 정보가 여러개임으로 한번에 worldBuffer로 관리한다.
	MatrixBuffer* worldBuffer;

	vector<Matrix> boneTransforms;
	map<string, Matrix> nodeTransforms;
};
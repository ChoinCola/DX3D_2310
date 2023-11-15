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

protected:
	string name;
	vector<Material*> materials;
	vector<ModelMesh*> meshes;
	vector<NodeData> nodes;

	// 모델과 메시의 정보가 여러개임으로 한번에 worldBuffer로 관리한다.
	MatrixBuffer* worldBuffer;

	vector<Matrix> boneTransforms;
	map<string, Matrix> nodeTransforms;
};
#pragma once

class ModelExporter
{
public:
	ModelExporter(string name, string file);
	~ModelExporter();

	void ExportMaterial();
	void ExportMesh();

private:
	//Material
	void ReadMaterial();// 머티리얼 읽기.
	void WriterMaterial(); // 머티얼 써서 저장하기.
	string CreateTexture(string file); // 텍스처 파일을 생성하기

	// Mesh
	void ReadNode(aiNode* node, int index, int parent); // 지정된 Ndoe값을 읽어오기
	void ReadMesh(aiNode* node);	// 메쉬읽기
	void WriteMesh();
private:
	Assimp::Importer* importer;
	const aiScene* scene;
	//aiScene 는 상수로 사용해야한다. 무조건.

	string name;

	vector<Material*> materials;
	vector<MeshData*> meshes;
	vector<NodeData*> nodes;
};
#pragma once

class ModelExporter
{
public:
	ModelExporter(string name, string file);
	~ModelExporter();

	void ExportMaterial();
	void ExportMesh();
	void ExportClip(string clipName);

private:
	//Material
	void ReadMaterial();// 머티리얼 읽기.
	void WriterMaterial(); // 머티얼 써서 저장하기.
	string CreateTexture(string file); // 텍스처 파일을 생성하기

	// Mesh
	void ReadBone(aiMesh* mesh, vector<VertexWeights>& vertexWeights);
	void ReadNode(aiNode* node, int index, int parent); // 지정된 Ndoe값을 읽어오기
	void ReadMesh(aiNode* node);	// 메쉬읽기
	void WriteMesh();

	//Animation
	Clip* ReadClip(aiAnimation* animation);
	void ReadKeyFrame(Clip* clip, aiNode* node, vector<ClipNode>& clopNodes);
	void WriteClip(Clip* clip, string clipName, UINT index);

private:
	void SetClipNode(const KeyData& keyData, const UINT& frameCount, ClipNode& clipNode);
	Float3 CalcInterpolationVector(const vector<KeyVector>& keyData, UINT& count, int curFrame);
	Float4 CalcInterpolationQuat(const vector<KeyQuat>& keyData, UINT& count, int curFrame);

private:
	Assimp::Importer* importer;
	const aiScene* scene;
	//aiScene 는 상수로 사용해야한다. 무조건.

	string name;

	vector<Material*> materials;
	vector<MeshData*> meshes;
	vector<NodeData*> nodes;
	vector<BoneData*> bones;

	map<string, UINT> boneMap;
	UINT boneCount = 0;
};
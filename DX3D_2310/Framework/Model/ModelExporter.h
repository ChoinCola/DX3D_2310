#pragma once

class ModelExporter
{
public:
	ModelExporter(string name, string file);
	~ModelExporter();

	void ExportMaterial();

private:
	//Material
	void ReadMaterial();// 머티리얼 읽기.
	void WriterMaterial(); // 머티얼 써서 저장하기.
	string CreateTexture(string file); // 텍스처 파일을 생성하기

private:
	Assimp::Importer* importer;
	const aiScene* scene;
	//aiScene 는 상수로 사용해야한다. 무조건.

	string name;

	vector<Material*> materials;
};
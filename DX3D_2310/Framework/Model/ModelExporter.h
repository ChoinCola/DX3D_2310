#pragma once

class ModelExporter
{
public:
	ModelExporter(string name, string file);
	~ModelExporter();

	void ExportMaterial();

private:
	//Material
	void ReadMaterial();// ��Ƽ���� �б�.
	void WriterMaterial(); // ��Ƽ�� �Ἥ �����ϱ�.
	string CreateTexture(string file); // �ؽ�ó ������ �����ϱ�

private:
	Assimp::Importer* importer;
	const aiScene* scene;
	//aiScene �� ����� ����ؾ��Ѵ�. ������.

	string name;

	vector<Material*> materials;
};
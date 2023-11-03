#pragma once

class Model : public Transform
{
public:
	Model(string name);
	~Model();

	void Render();
	void GUIRender();

	void SetShader(wstring file);

private:
	void ReadMaterial();
	void ReadMesh();

protected:
	string name;
	vector<Material*> materials;
	vector<ModelMesh*> meshes;

	// 모델과 메시의 정보가 여러개임으로 한번에 worldBuffer로 관리한다.
	MatrixBuffer* worldBuffer;
};
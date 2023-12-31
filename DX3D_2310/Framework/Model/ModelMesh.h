#pragma once

class ModelMesh
{
public:
	ModelMesh(string name);
	~ModelMesh();

	void Render();
	void RenderInstanced(UINT count);
	void CreateMesh(void* vertices, UINT vertexCount,
		void* indices, UINT indexCount);

	void SetMaterial(Material* material, UINT index) { 
		this->material = material;
		data.materialIndex = index;
	}
	// 모델에 있는것을 참조받을것이다.

	string GetName() { return name; }
	MeshData GetData() { return data; }
	BlendState* GetBlendState() { return blendState[1]; }
	RasterizerState* GetRasterizerState() { return rasterizerstate[1]; }
private:
	string name;

	Material* material;
	Mesh<ModelVertex>* mesh;

	MeshData data;

	BlendState* blendState[2];
	RasterizerState* rasterizerstate[2];
};
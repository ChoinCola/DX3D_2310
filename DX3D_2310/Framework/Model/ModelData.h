#pragma once
typedef VertexUVNormalTangent ModelVertex;

// 일반적인 데이터와 같으나, Model전용 meshData를 저장하는 구조체가 따로 필요하다.
// 모델에는 materialIndex데이터가 들어가야하고, 일반적인 Mesh구조체와 다르게 작성되기 때문,
class MeshData
{
public:
	string name;
	UINT materialIndex;

	vector<ModelVertex> vertices;
	vector<UINT> indices;
};
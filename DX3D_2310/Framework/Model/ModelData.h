#pragma once
typedef VertexUVNormalTangent ModelVertex;

class MeshData
{
public:
	string name;
	UINT materialIndex;

	vector<ModelVertex> vertices;
	vector<UINT> indices;
};
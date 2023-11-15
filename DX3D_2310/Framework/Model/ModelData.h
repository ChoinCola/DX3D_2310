#pragma once
typedef VertexUVNormalTangent ModelVertex;

// �Ϲ����� �����Ϳ� ������, Model���� meshData�� �����ϴ� ����ü�� ���� �ʿ��ϴ�.
// �𵨿��� materialIndex�����Ͱ� �����ϰ�, �Ϲ����� Mesh����ü�� �ٸ��� �ۼ��Ǳ� ����,
class MeshData
{
public:
	string name;
	UINT materialIndex;

	vector<ModelVertex> vertices;
	vector<UINT> indices;
};

struct NodeData
{
	int index;
	string name;
	int parent;
	Matrix transform;
};
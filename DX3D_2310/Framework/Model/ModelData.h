#pragma once
typedef VertexUVNormalTangentBlend ModelVertex;

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

struct BoneData
{
	int index;
	string name;
	Matrix offset;
};

// vertice�� �������� ä��� ���� ����������.
struct VertexWeights
{
	UINT indices[4] = {};
	float weights[4] = {}; // ����ġ

	void Add(const UINT& index, const float& weight)
	{
		FOR(4)
		{
			if (weights[i] == 0.0f)
			{
				indices[i] = index;
				weights[i] = weight;
				return;
			}
		}
	}

	void Normalize()
	{
		float sum = 0.0f;

		FOR(4)
			sum += weights[i];

		// ǥ�������� 1�� ����ȭ�Ѵ�.
		FOR(4)
			weights[i] /= sum;
	}
};

// �����ʹ� �̹������·� ���� ���޵ȴ�.

struct KeyVector
{
	float time;
	Float3 value;

};

struct KeyQuat
{
	float time;
	Float4 value;

};

struct KeyData
{
	vector<KeyVector> scales;
	vector<KeyQuat> rotations;
	vector<KeyVector> positions;
};

struct KeyTransform
{
	Float3 scale;
	Float4 rot;
	Float3 pos;
};

struct KeyFrame
{
	string boneName;
	vector<KeyTransform> transforms;
};

struct ClipNode
{
	aiString name;
	vector<KeyTransform> transforms;
};

struct Clip
{
	string name;
	UINT frameCount;
	float tickPerSecond;

	vector<KeyFrame*> keyFrames;
};
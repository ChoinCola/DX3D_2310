#pragma once
typedef VertexUVNormalTangentBlend ModelVertex;

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

// vertice의 보간값을 채우기 위한 데이터정보.
struct VertexWeights
{
	UINT indices[4] = {};
	float weights[4] = {}; // 가중치

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

		// 표준편차로 1로 정규화한다.
		FOR(4)
			weights[i] /= sum;
	}
};

// 데이터는 이미지형태로 저장 전달된다.

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
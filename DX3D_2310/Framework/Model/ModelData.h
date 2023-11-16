#pragma once
typedef VertexUVNormalTangentBlend ModelVertex;

// 일반적인 데이터와 같으나, Model전용 meshData를 저장하는 구조체가 따로 필요하다.
// 모델에는 materialIndex데이터가 들어가야하고, 일반적인 Mesh구조체와 다르게 작성되기 때문,

// MeshData 클래스는 모델의 메시 데이터를 저장하는 클래스입니다.
class MeshData
{
public:
	string name;             // 메시의 이름
	UINT materialIndex;      // 메시에 적용된 재질의 인덱스

	vector<ModelVertex> vertices;  // 메시의 정점 데이터
	vector<UINT> indices;         // 메시의 인덱스 데이터
};

// NodeData 구조체는 모델의 노드(뼈대) 데이터를 저장하는 구조체입니다.
struct NodeData
{
	int index;         // 노드(뼈대)의 인덱스
	string name;       // 노드(뼈대)의 이름
	int parent;        // 부모 노드(뼈대)의 인덱스
	Matrix transform;  // 노드(뼈대)의 변환 행렬
};

// BoneData 구조체는 모델의 뼈대(Bone) 데이터를 저장하는 구조체입니다.
struct BoneData
{
	int index;         // 뼈대의 인덱스
	string name;       // 뼈대의 이름
	Matrix offset;     // 뼈대의 로컬 변환 행렬
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

// KeyVector 구조체는 애니메이션의 키프레임에서 위치, 회전, 스케일의 값을 저장하는 구조체입니다.
struct KeyVector
{
	float time;   // 키프레임의 시간 정보
	Float3 value;  // 위치, 회전, 스케일 값
};

// KeyQuat 구조체는 애니메이션의 키프레임에서 쿼터니언 회전값을 저장하는 구조체입니다.
struct KeyQuat
{
	float time;   // 키프레임의 시간 정보
	Float4 value;  // 쿼터니언 회전값
};

// KeyData 구조체는 애니메이션의 키프레임 데이터를 저장하는 구조체입니다.
struct KeyData
{
	vector<KeyVector> scales;    // 스케일 키프레임 데이터
	vector<KeyQuat> rotations;   // 회전 키프레임 데이터
	vector<KeyVector> positions; // 위치 키프레임 데이터
};

// KeyTransform 구조체는 애니메이션의 키프레임에서 변환 정보(스케일, 회전, 위치)를 저장하는 구조체입니다.
struct KeyTransform
{
	Float3 scale;  // 스케일 정보
	Float4 rot;    // 회전 정보
	Float3 pos;    // 위치 정보
};

// KeyFrame 구조체는 특정 뼈대에 대한 애니메이션의 키프레임 데이터를 저장하는 구조체입니다.
struct KeyFrame
{
	string boneName;            // 뼈대의 이름
	vector<KeyTransform> transforms;  // 키프레임에서의 변환 정보들
};

// ClipNode 구조체는 에니메이션 클립에서의 노드(뼈대) 정보를 저장하는 구조체입니다.
struct ClipNode
{
	aiString name;              // 노드(뼈대)의 이름
	vector<KeyTransform> transforms;  // 노드에서의 키프레임에서의 변환 정보들
};

// Clip 구조체는 애니메이션 클립에 대한 정보를 저장하는 구조체입니다.
struct Clip
{
	string name;                  // 클립의 이름
	UINT frameCount;              // 총 프레임 수
	float tickPerSecond;          // 초당 틱 수
	vector<KeyFrame*> keyFrames;  // 클립에서의 키프레임 데이터들
};
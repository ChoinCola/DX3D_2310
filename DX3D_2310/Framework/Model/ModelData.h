#pragma once
typedef VertexUVNormalTangentBlend ModelVertex;

// �Ϲ����� �����Ϳ� ������, Model���� meshData�� �����ϴ� ����ü�� ���� �ʿ��ϴ�.
// �𵨿��� materialIndex�����Ͱ� �����ϰ�, �Ϲ����� Mesh����ü�� �ٸ��� �ۼ��Ǳ� ����,

// MeshData Ŭ������ ���� �޽� �����͸� �����ϴ� Ŭ�����Դϴ�.
class MeshData
{
public:
	string name;             // �޽��� �̸�
	UINT materialIndex;      // �޽ÿ� ����� ������ �ε���

	vector<ModelVertex> vertices;  // �޽��� ���� ������
	vector<UINT> indices;         // �޽��� �ε��� ������
};

// NodeData ����ü�� ���� ���(����) �����͸� �����ϴ� ����ü�Դϴ�.
struct NodeData
{
	int index;         // ���(����)�� �ε���
	string name;       // ���(����)�� �̸�
	int parent;        // �θ� ���(����)�� �ε���
	Matrix transform;  // ���(����)�� ��ȯ ���
};

// BoneData ����ü�� ���� ����(Bone) �����͸� �����ϴ� ����ü�Դϴ�.
struct BoneData
{
	int index;         // ������ �ε���
	string name;       // ������ �̸�
	Matrix offset;     // ������ ���� ��ȯ ���
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

// KeyVector ����ü�� �ִϸ��̼��� Ű�����ӿ��� ��ġ, ȸ��, �������� ���� �����ϴ� ����ü�Դϴ�.
struct KeyVector
{
	float time;   // Ű�������� �ð� ����
	Float3 value;  // ��ġ, ȸ��, ������ ��
};

// KeyQuat ����ü�� �ִϸ��̼��� Ű�����ӿ��� ���ʹϾ� ȸ������ �����ϴ� ����ü�Դϴ�.
struct KeyQuat
{
	float time;   // Ű�������� �ð� ����
	Float4 value;  // ���ʹϾ� ȸ����
};

// KeyData ����ü�� �ִϸ��̼��� Ű������ �����͸� �����ϴ� ����ü�Դϴ�.
struct KeyData
{
	vector<KeyVector> scales;    // ������ Ű������ ������
	vector<KeyQuat> rotations;   // ȸ�� Ű������ ������
	vector<KeyVector> positions; // ��ġ Ű������ ������
};

// KeyTransform ����ü�� �ִϸ��̼��� Ű�����ӿ��� ��ȯ ����(������, ȸ��, ��ġ)�� �����ϴ� ����ü�Դϴ�.
struct KeyTransform
{
	Float3 scale;  // ������ ����
	Float4 rot;    // ȸ�� ����
	Float3 pos;    // ��ġ ����
};

// KeyFrame ����ü�� Ư�� ���뿡 ���� �ִϸ��̼��� Ű������ �����͸� �����ϴ� ����ü�Դϴ�.
struct KeyFrame
{
	string boneName;            // ������ �̸�
	vector<KeyTransform> transforms;  // Ű�����ӿ����� ��ȯ ������
};

// ClipNode ����ü�� ���ϸ��̼� Ŭ�������� ���(����) ������ �����ϴ� ����ü�Դϴ�.
struct ClipNode
{
	aiString name;              // ���(����)�� �̸�
	vector<KeyTransform> transforms;  // ��忡���� Ű�����ӿ����� ��ȯ ������
};

// Clip ����ü�� �ִϸ��̼� Ŭ���� ���� ������ �����ϴ� ����ü�Դϴ�.
struct Clip
{
	string name;                  // Ŭ���� �̸�
	UINT frameCount;              // �� ������ ��
	float tickPerSecond;          // �ʴ� ƽ ��
	vector<KeyFrame*> keyFrames;  // Ŭ�������� Ű������ �����͵�
};
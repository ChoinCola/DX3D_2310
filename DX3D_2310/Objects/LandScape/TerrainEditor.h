#pragma once

class TerrainEditor : public GameObject
{
private:
	//typedef VertexUVNormal VertexType;
	// VertexType�� �����մϴ�.
	// VertexType�� ��ġ, �ؽ�ó ��ǥ, ���� ���� �� alpha ������ �����˴ϴ�.
	struct VertexType
	{
		Float3 pos = {};
		Float2 uv = {};
		Float3 normal = {};
		float alpha[4] = {};
	};

	enum EditType
	{
		HEIGHT, ALPHA
	};

	// ���� �������� �ִ� ũ�� �� �ִ� ���̸� ����� �����մϴ�.
	const UINT MAX_SIZE = 256;
	const UINT MAX_HEIGHT = 20.0f;

	// RayBuffer�� ���� �����⿡�� ����ĳ������ �����ϱ� ���� ��� �����Դϴ�.
	class RayBuffer : public ConstBuffer
	{
	private:
		struct Data
		{
			Float3 pos;
			UINT triangleSize;
			Float3 dir;
			float padding;
		};

	public:
		RayBuffer() : ConstBuffer(&data, sizeof(Data))
		{
		}

		Data& Get() { return data; }

	private:
		Data data;
	};

	// BrushBuffer�� ������ ������ �� ���Ǵ� �귯���� �Ӽ��� ��� ��� �����Դϴ�.
	class BrushBuffer : public ConstBuffer
	{
	private:
		struct Data
		{
			int type = 0;
			Float3 pickingPos;
			float range = 20.0f;
			Float3 color = { 0, 1, 0 };
		};

	public:
		BrushBuffer() : ConstBuffer(&data, sizeof(Data))
		{
		}

		Data& Get() { return data; }

	private:
		Data data;
	};

	// �Է°� ��� ����ü�� �����մϴ�.
	// �Է� ����ü�� �ﰢ���� �� ������ ��ǥ�� ��Ÿ���ϴ�.
	// ��� ����ü�� ��ŷ ���ο� �Ÿ��� ��Ÿ���ϴ�.
	struct InputDesc
	{
		Float3 v0, v1, v2;
	};

	struct OutputDesc
	{
		int picked;
		float distance;
	};

public:
	// ���� �������� �����ڿ� �Ҹ��ڸ� �����մϴ�.
	TerrainEditor();
	~TerrainEditor();

	// ���� �������� ������Ʈ, ������, GUI ������, ��ŷ, ��ǻƮ ������ ���� ���� ����� �����մϴ�.
	void Update();
	void Render();
	void GUIRender();
	void Picking();
	bool ComputePicking(Vector3& pos);
	void MakeComputeData();

	// ���� ��ġ���� �߷� ���ӵ��� ����ϴ� �Լ��� �����մϴ�.
	Vector3 GetOngravityAcceleration(const Vector3 ObjectPos, const Vector3 correction = { 0, 0, 0 });

	// ���� ��Ҵ��� ���θ� Ȯ���ϴ� �Լ��� ������ �ʺ� �� ���̸� ��ȯ�ϴ� �Լ��� �����մϴ�.
	bool ChackOnGround(const Vector3 ObjectPos);
	int GetWidth() { return width; }
	int GetHeight() { return height; }

private:
	// ������ ã�� �Լ��� �޽� ����, ���� �� �� ���� �� ���� �� �ε� �Լ�, ���� ���� �� ���� ���� �Լ��� �����մϴ�.
	pair<Vector3, Vector3> FindLowPos(Vector3 e0, Vector3 e1, Vector3 e2, Vector3 e3);
	void MakeMesh(bool tile = false, bool flip = false);
	void MakeNormal(bool Flip = false);
	void AdjustHeight();
	void AdjustAlpha();
	void SaveHeightMap();
	void LoadHeightMap();
	void SaveAlphaMap();
	void LoadAlphaMap();
	void UpdateHeight();
	void Resize();

private:
	// ��� �������� �����մϴ�.
	UINT width, height, triangleSize;
	Vector3 pickingPos;
	float adjustValue = 20.0f;
	string projectPath;
	EditType editType = HEIGHT;
	int selectMap = 0;

	Mesh<VertexType>* mesh;
	Mesh<VertexColor>* normalline;

	Texture* heightMap;
	Texture* secondMap;
	Texture* thirdMap;

	Vector3 defaltsize = {};
	RayBuffer* rayBuffer;
	BrushBuffer* brushBuffer;
	StructuredBuffer* structuredBuffer;
	vector<InputDesc> inputs;
	vector<OutputDesc> outputs;
	ComputeShader* computeShader;
};
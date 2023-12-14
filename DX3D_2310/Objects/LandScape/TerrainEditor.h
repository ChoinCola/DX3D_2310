#pragma once

class TerrainEditer : public GameObject
{
private:
	typedef VertexUVNormal VertexType;

	const UINT MAX_SIZE = 256;

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
	TerrainEditer();

	~TerrainEditer();

	void Update();
	void Render();
	void GUIRender();
	void Picking();
	bool ComputePicking(Vector3& pos);
	void MakeComputeData();

	// 현재 위치에서 중력가속도 구하는 함수
	Vector3 GetOngravityAcceleration(const Vector3 ObjectPos, const Vector3 correction = { 0, 0, 0 });

	bool ChackOnGround(const Vector3 ObjectPos);
	int GetWidth() { return width; }
	int GetHeight() { return height; }

private:
	pair<Vector3, Vector3> FindLowPos(Vector3 e0, Vector3 e1, Vector3 e2, Vector3 e3);
	void MakeMesh(bool tile = false, bool flip = false);
	void MakeNormal(bool Flip = false);

private:
	UINT width, height, triangleSize;
	Vector3 pickingPos;
	Mesh<VertexType>* mesh;
	Mesh<VertexColor>* normalline;

	Texture* heightMap;
	Vector3 defaltsize = {};

	RayBuffer* rayBuffer;
	StructuredBuffer* structuredBuffer;

	vector<InputDesc> inputs;
	vector<OutputDesc> outputs;

	ComputeShader* computeShader;
};
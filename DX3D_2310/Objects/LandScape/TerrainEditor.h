#pragma once

class TerrainEditor : public GameObject
{
private:
	//typedef VertexUVNormal VertexType;
	// VertexType을 정의합니다.
	// VertexType은 위치, 텍스처 좌표, 법선 벡터 및 alpha 값으로 구성됩니다.
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

	// 지형 편집기의 최대 크기 및 최대 높이를 상수로 정의합니다.
	const UINT MAX_SIZE = 256;
	const UINT MAX_HEIGHT = 20.0f;

	// RayBuffer는 지형 편집기에서 레이캐스팅을 수행하기 위한 상수 버퍼입니다.
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

	// BrushBuffer는 지형을 편집할 때 사용되는 브러시의 속성을 담는 상수 버퍼입니다.
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

	// 입력과 출력 구조체를 정의합니다.
	// 입력 구조체는 삼각형의 세 꼭지점 좌표를 나타냅니다.
	// 출력 구조체는 피킹 여부와 거리를 나타냅니다.
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
	// 지형 편집기의 생성자와 소멸자를 선언합니다.
	TerrainEditor();
	~TerrainEditor();

	// 지형 편집기의 업데이트, 렌더링, GUI 렌더링, 피킹, 컴퓨트 데이터 생성 등의 기능을 정의합니다.
	void Update();
	void Render();
	void GUIRender();
	void Picking();
	bool ComputePicking(Vector3& pos);
	void MakeComputeData();

	// 현재 위치에서 중력 가속도를 계산하는 함수를 정의합니다.
	Vector3 GetOngravityAcceleration(const Vector3 ObjectPos, const Vector3 correction = { 0, 0, 0 });

	// 땅에 닿았는지 여부를 확인하는 함수와 지형의 너비 및 높이를 반환하는 함수를 정의합니다.
	bool ChackOnGround(const Vector3 ObjectPos);
	int GetWidth() { return width; }
	int GetHeight() { return height; }

private:
	// 저점을 찾는 함수와 메시 생성, 높이 맵 및 알파 맵 저장 및 로드 함수, 높이 조절 및 알파 조절 함수를 정의합니다.
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
	// 멤버 변수들을 정의합니다.
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
#include "Framework.h"

TerrainEditer::TerrainEditer() : width(MAX_SIZE), height(MAX_SIZE)
{
	material->SetDiffuseMap(L"Textures/Landscape/Dirt2.png");
	mesh = new Mesh<VertexType>;
	MakeMesh();
	MakeNormal();
	MakeComputeData();
	mesh->CreateMesh();

	computeShader = Shader::AddCS(L"Compute/ComputePicking.hlsl");

	structuredBuffer = new StructuredBuffer(
		inputs.data(), sizeof(InputDesc), triangleSize,
		sizeof(OutputDesc), triangleSize);
	rayBuffer = new RayBuffer();
}

TerrainEditer::~TerrainEditer()
{
	delete mesh;
}

void TerrainEditer::Update()
{
	//Picking();
	ComputePicking(pickingPos);
}

void TerrainEditer::Render()
{
	SetRender();
	mesh->Draw();
}

void TerrainEditer::GUIRender()
{
	string def = pickingPos;
	ImGui::Text(def.c_str());
}

void TerrainEditer::Picking()
{
	Ray ray = CAM->ScreenPointToRay(mousePos);

	for (UINT z = 0; z < height - 1; z++)
	{
		for (UINT x = 0; x < width - 1; x++)
		{
			UINT index[4];
			index[0] = width * z + x;
			index[1] = width * z + x + 1;
			index[2] = width * (z + 1) + x;
			index[3] = width * (z + 1) + x + 1;

			vector<VertexType>& vertices = mesh->GetVertices();

			Vector3 p[4];
			FOR(4)
				p[i] = vertices[index[i]].pos;

			float distance = 0.0f;

			if (Intersects(ray.pos, ray.dir, p[0], p[1], p[2], distance))
			{
				pickingPos = ray.pos + ray.dir * distance;
				return;
			}
			if (Intersects(ray.pos, ray.dir, p[3], p[1], p[2], distance))
			{
				pickingPos = ray.pos + ray.dir * distance;
				return;
			}
		}
	}
}

bool TerrainEditer::ComputePicking(Vector3& pos)
{
	// 마우스 위치로부터 레이 생성
	Ray ray = CAM->ScreenPointToRay(mousePos);

	// 레이와 삼각형의 교차 여부를 병렬처리하기 위해 필요한 데이터 설정
	rayBuffer->Get().pos = ray.pos;
	rayBuffer->Get().dir = ray.dir;
	rayBuffer->Get().triangleSize = triangleSize;

	// 레이 버퍼를 Compute Shader에 전달
	rayBuffer->SetCS(0);

	// 구조화된 버퍼를 Compute Shader에서 읽기 위한 SRV로 설정
	DC->CSSetShaderResources(0, 1, &structuredBuffer->GetSRV());

	// 구조화된 버퍼를 Compute Shader에서 쓰기 위한 UAV로 설정
	DC->CSSetUnorderedAccessViews(0, 1, &structuredBuffer->GetUAV(), nullptr);

	// Compute Shader를 설정
	computeShader->Set();

	// 쓰레드 그룹 수 계산 (올림 처리)
	UINT x = ceil((float)triangleSize / 64.0f);

	// Compute Shader 실행
	DC->Dispatch(x, 1, 1);
	//---------------------------------------
	// 이 이후에 outputs데이터에 데이터처리결과가 저장된다.


	// 결과를 CPU로 복사
	structuredBuffer->Copy(outputs.data(), sizeof(OutputDesc) * triangleSize);

	// 최소 거리와 해당 인덱스 초기화
	float minDistance = FLT_MAX;
	int minIndex = -1;

	// 결과를 분석하여 최소 거리를 찾음
	UINT index = 0;
	for (OutputDesc output : outputs)
	{
		if (output.picked)
		{
			if (minDistance > output.distance)
			{
				minDistance = output.distance;
				minIndex = index;
			}
		}
		index++;
	}

	// 최소 거리가 업데이트되었다면 교차 지점을 계산하고 true 반환
	if (minIndex >= 0)
	{
		pos = ray.pos + ray.dir * minDistance;
		return true;
	}

	// 교차 지점이 없을 경우 false 반환
	return false;
}


void TerrainEditer::MakeComputeData()
{
	vector<VertexType> vertices = mesh->GetVertices();
	vector<UINT> indices = mesh->GetIndices();

	triangleSize = indices.size() / 3;

	inputs.resize(triangleSize);
	outputs.resize(triangleSize);

	for (UINT i = 0; i < triangleSize; i++)
	{
		UINT index0 = indices[i * 3 + 0];
		UINT index1 = indices[i * 3 + 1];
		UINT index2 = indices[i * 3 + 2];

		inputs[i].v0 = vertices[index0].pos;
		inputs[i].v1 = vertices[index1].pos;
		inputs[i].v2 = vertices[index2].pos;
	}
}

Vector3 TerrainEditer::GetOngravityAcceleration(const Vector3 ObjectPos, const Vector3 correction)
{
    return Vector3();
}

bool TerrainEditer::ChackOnGround(const Vector3 ObjectPos)
{
    return false;
}

pair<Vector3, Vector3> TerrainEditer::FindLowPos(Vector3 e0, Vector3 e1, Vector3 e2, Vector3 e3)
{
    return pair<Vector3, Vector3>();
}

void TerrainEditer::MakeMesh(bool tile, bool flip)
{
	vector<Float4> pixels(width * height, Float4(0, 0, 0, 0));
	if (heightMap)
	{
		width = heightMap->GetSize().x;
		height = heightMap->GetSize().y;

		heightMap->ReadPixels(pixels);
	}
	//Vertices
	vector<VertexType>& vertices = mesh->GetVertices();
	vertices.clear();

	vertices.reserve(width * height);
	for (UINT z = 0; z < height; z++)
	{
		for (UINT x = 0; x < width; x++)
		{
			VertexType vertex;
			vertex.pos = { (float)x, 0.0f, (float)z };
			vertex.uv.x = x / (float)(width - 1);
			vertex.uv.y = z / (float)(height - 1);

			UINT index = width * z + x;
			vertex.pos.y = pixels[index].x * 20.0f;

			vertices.push_back(vertex);
		}
	}

	//Indices
	vector<UINT>& indices = mesh->GetIndices();
	indices.clear();

	indices.reserve((width - 1) * (height - 1) * 6);

	for (UINT z = 0; z < height - 1; z++)
	{
		for (UINT x = 0; x < width - 1; x++)
		{
			indices.push_back(width * z + x);//0
			indices.push_back(width * (z + 1) + x);//1
			indices.push_back(width * z + x + 1);//2

			indices.push_back(width * z + x + 1);//2
			indices.push_back(width * (z + 1) + x);//1
			indices.push_back(width * (z + 1) + x + 1);//3
		}
	}
}

void TerrainEditer::MakeNormal(bool Flip)
{
	vector<VertexType>& vertices = mesh->GetVertices();
	vector<UINT>& indices = mesh->GetIndices();

	FOR(indices.size() / 3)
	{
		UINT index0 = indices[i * 3 + 0];
		UINT index1 = indices[i * 3 + 1];
		UINT index2 = indices[i * 3 + 2];

		Vector3 v0 = vertices[index0].pos;
		Vector3 v1 = vertices[index1].pos;
		Vector3 v2 = vertices[index2].pos;

		Vector3 e0 = v1 - v0;
		Vector3 e1 = v2 - v0;

		Vector3 noraml = Vector3::Cross(e0, e1).GetNormalized();

		if (Flip) {
			vertices[index0].normal -= noraml;
			vertices[index1].normal -= noraml;
			vertices[index2].normal -= noraml;
		}
		else
		{
			vertices[index0].normal += noraml;
			vertices[index1].normal += noraml;
			vertices[index2].normal += noraml;
		}
	}
}

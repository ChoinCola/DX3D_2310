#include "Framework.h"

Terrain::Terrain()
{
	SetLocalPosition({ 0, 0, 0 });
	tag = "Terrain";
	material->SetShader(L"LandScape/Terrain.hlsl");
	material->SetDiffuseMap(L"Textures/Colors/White.png");

	heightMap = Texture::Add(L"Textures/HeightMaps/HeightMap.png");
	alphaMap = Texture::Add(L"Textures/HeightMaps/AlphaMap.png");
	secondMap = Texture::Add(L"Textures/Landscape/Dirt3.png");

	mesh = new Mesh<VertexUVNormal>();
	normalline = new Mesh<VertexColor>();
	RSset = new RasterizerState();

	MakeMesh();
	MakeNormal();
	mesh->CreateMesh();
	normalline->CreateMesh();
	RSset->SetState();
}

Terrain::Terrain(const wstring hightmap, const float hight, bool tile, bool flip)
	: hight(hight)
{
	SetLocalPosition({ 0, 0, 0 });
	tag = "Terrain";
	material->SetShader(L"LandScape/Terrain.hlsl");
	material->SetDiffuseMap(L"Textures/Colors/White.png");

	heightMap = Texture::Add(L"Textures/HeightMaps/HeightMap.png");
	alphaMap = Texture::Add(L"Textures/HeightMaps/AlphaMap.png");
	secondMap = Texture::Add(L"Textures/Landscape/Dirt3.png");

	mesh = new Mesh<VertexUVNormal>();
	normalline = new Mesh<VertexColor>();
	RSset = new RasterizerState();

	MakeMesh(tile, flip);
	MakeNormal(flip);
	mesh->CreateMesh();
	normalline->CreateMesh();
	RSset->SetState();
}
Terrain::~Terrain()
{
	delete mesh;
}

void Terrain::Render()
{
	alphaMap->PSSet(10);
	secondMap->PSSet(11);

	SetRender();
	mesh->Draw();
}

void Terrain::GUIRender()
{
	ImGui::Text("x: %d, z: %d, y:%d", x, z, y);
	ImGui::Text("normalize : x: %d, y: %d, z: %d", v.x, v.y, v.z);
	ImGui::Text("D : %d", D);

	__super::GUIRender();
}

Vector3 Terrain::GetOnGrondPosition(const Vector3 ObjectPos, const Vector3 correction)
{
	// 범위 밖 인경우 자기자신 반환
	// 좌표는 항상 맵위치의 상대위치로 계산한다.
	Vector3 LocalMaxPos =  Vector3(width, 0, height) + GetLocalPosition();
	Vector3 LocalLowPos = GetLocalPosition();
	x = (int)ObjectPos.x;
	z = (int)ObjectPos.z;

	if (x < 0 || x >= width - 1)		return ObjectPos;
	if (z < 0 || z >= height - 1)		return ObjectPos;

	// 범위 안 일경우,
	// 현재 Object위치를 XY좌표로 반환하여 어떠한 사각형 안에 들어와있는지 판별함.
	// 기본 디폴트 칸 사이즈는 xy가 1임
	// 대충 xy좌표를 int로 변환하면 현재 Pos의 칸 내부가 나타남.
	vector<VertexType>& vertices = mesh->GetVertices();
	vector<UINT>& indices = mesh->GetIndices();


	// indicees를 참조해서 데이터를 구했으면, 3과 0중 더 가까운vertices를 찾아야함.
	// 사각형이 아닌 삼각형으로 vertices가 그어졌음으로 삼각형 기준으로 높이값을 구해야하기 때문.
	UINT index[4];
	index[0] = width * z + x;
	index[1] = width * (z + 1) + x;
	index[2] = width * z + x + 1;
	index[3] = width * (z + 1) + x + 1;

	Vector3 e0 = vertices[index[0]].pos;	// 0
	Vector3 e1 = vertices[index[1]].pos;	// 1
	Vector3 e2 = vertices[index[2]].pos;	// 2
	Vector3 e3 = vertices[index[3]].pos;	// 3

	// 스칼라값을 구해줌. e0이 더 가까울경우, e0으로 진행
	if (ObjectPos.x <  ObjectPos.z)
	{
		// e0e1, e0e2 외적.
		v = Vector3::ComputeNormal(e0, e1, e2);
		//평면방정식,
		// x + y + z + D = 0;
		// D 구하기.
		// D = -Ax1 - By1 - Cz1; 원점에서 평면까지의 거리
		// D = -v.x * e0.x + -v.y * e0.y + -v.z * e0.z
		D = -v.x * e0.x - v.y * e0.y - v.z * e0.z;
		// 수식 재정렬. y = - x - z - D
		y = -(v.x * ObjectPos.x + v.z * ObjectPos.z + D) / v.y;

		return Vector3(ObjectPos.x, y, ObjectPos.z) + correction;

	}
	// e3와 더 가깝거나 같을경우, e3로 진행.
	else
	{
		// e0e1, e0e2 외적.
		v = Vector3::ComputeNormal(e3, e1, e2);
		//평면방정식,
		// x + y + z + D = 0;
		// D 구하기.
		// D = -Ax1 - By1 - Cz1;
		// D = -v.x * e0.x + -v.y * e0.y + -v.z * e0.z
		D = -v.x * e3.x - v.y * e3.y - v.z * e3.z;
		// 수식 재정렬. y = - x - z - D
		y = -(v.x * ObjectPos.x + v.z * ObjectPos.z + D) / v.y;

		return Vector3(ObjectPos.x, y, ObjectPos.z) + correction;
	}

}
Vector3 Terrain::GetOngravityAcceleration(const Vector3 ObjectPos, const Vector3 correction)
{	
	// 범위 밖 인경우 자기자신 반환
	// 좌표는 항상 맵위치의 상대위치로 계산한다.
	Vector3 LocalMaxPos = Vector3(width, 0, height) + GetLocalPosition();
	Vector3 LocalLowPos = GetLocalPosition();

	x = (int)ObjectPos.x;
	z = (int)ObjectPos.z;

	if (x < 0 || x >= width - 1)		return Vector3(0, 0, 0);
	if (z < 0 || z >= height - 1)		return Vector3(0, 0, 0);

	// 범위 안 일경우,
	// 현재 Object위치를 XY좌표로 반환하여 어떠한 사각형 안에 들어와있는지 판별함.

	// 기본 디폴트 칸 사이즈는 xy가 1임
	// 대충 xy좌표를 int로 변환하면 현재 Pos의 칸 내부가 나타남.
	vector<VertexType>& vertices = mesh->GetVertices();
	vector<UINT>& indices = mesh->GetIndices();


	// indicees를 참조해서 데이터를 구했으면, 3과 0중 더 가까운vertices를 찾아야함.
	// 사각형이 아닌 삼각형으로 vertices가 그어졌음으로 삼각형 기준으로 높이값을 구해야하기 때문.
	UINT index[4];
	index[0] = width * z + x;
	index[1] = width * (z + 1) + x;
	index[2] = width * z + x + 1;
	index[3] = width * (z + 1) + x + 1;

	Vector3 e0 = vertices[index[0]].pos;	// 0
	Vector3 e1 = vertices[index[1]].pos;	// 1
	Vector3 e2 = vertices[index[2]].pos;	// 2
	Vector3 e3 = vertices[index[3]].pos;	// 3

	// 현재 높이값이 같거나 작을경우를 확인
	if (e0.y == e1.y && e1.y == e2.y && e2.y == e3.y)	return Vector3(0, 0, 0);

	// 스칼라값을 구해줌. e0이 더 가까울경우, e0으로 진행
	pair<Vector3, Vector3> Lowypos = FindLowPos(e0, e1, e2, e3);

	Vector3 direction = Lowypos.second - Lowypos.first;

	Vector3 posdirection = ObjectPos;
	posdirection -= Lowypos.first;

	// 가장 밑에있는 선분에 현재 Pos를 투영해줌
	float intersection = Vector3::Dot(direction, posdirection);
	direction = direction * intersection;

	// 현재 방향벡터 (아래로 향하는 방향)
	Vector3 gravityDirection = (Lowypos.first + direction + correction) - ObjectPos;
	gravityDirection.Normalized(); // 노멀라이즈해줌

	return gravityDirection;

}
Vector3 Terrain::Picking()
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
				return ray.pos + ray.dir * distance;
			}
			if (Intersects(ray.pos, ray.dir, p[3], p[1], p[2], distance))
			{
				return ray.pos + ray.dir * distance;
			}
		}
	}
}
bool Terrain::ChackOnGround(const Vector3 ObjectPos)
{
	x = (int)ObjectPos.x;
	z = (int)ObjectPos.z;
	if (x < 0 || x >= width - 1)		return false;
	if (z < 0 || z >= height - 1)		return false;

	return true;
}

pair<Vector3, Vector3> Terrain::FindLowPos(Vector3 e0, Vector3 e1, Vector3 e2, Vector3 e3)
{
	// 가장 낮은 높이를 가진 vertex와 그 다음으로 낮은 높이를 가진 vertex를 찾기 위한 초기값 설정
	Vector3 lowestHeightVertex = e0;
	Vector3 secondLowestHeightVertex = e1;

	// e1의 높이가 더 낮은 경우
	if (e1.y < lowestHeightVertex.y) {
		secondLowestHeightVertex = lowestHeightVertex;
		lowestHeightVertex = e1;
	}
	else if (e1.y < secondLowestHeightVertex.y) {
		secondLowestHeightVertex = e1;
	}

	// e2의 높이가 더 낮은 경우
	if (e2.y < lowestHeightVertex.y) {
		secondLowestHeightVertex = lowestHeightVertex;
		lowestHeightVertex = e2;
	}
	else if (e2.y < secondLowestHeightVertex.y) {
		secondLowestHeightVertex = e2;
	}

	// e3의 높이가 더 낮은 경우
	if (e3.y < lowestHeightVertex.y) {
		secondLowestHeightVertex = lowestHeightVertex;
		lowestHeightVertex = e3;
	}
	else if (e3.y < secondLowestHeightVertex.y) {
		secondLowestHeightVertex = e3;
	}

	// 가장 낮은 높이를 가진 vertex와 그 다음으로 낮은 높이를 가진 vertex 반환
	return make_pair(lowestHeightVertex, secondLowestHeightVertex);
}

void Terrain::MakeMesh(bool tile, bool flip)
{
	width = heightMap->GetSize().x;
	height = heightMap->GetSize().y;
	if (width == 0) width = 10;
	if (height == 0) height = 10;

	vector<Float4> pixels;
	heightMap->ReadPixels(pixels);
	bool defaltpixels = false;
	if (!pixels.empty()) defaltpixels = true;
	vector<VertexType>& vertices = mesh->GetVertices();

	vertices.reserve(width * height);
	for (UINT z = 0; z < height; z++) {
		for (UINT x = 0; x < width; x++) {
			VertexType vertex;
			vertex.pos = { (float)x, 0.0f, (float)z };
			vertex.uv.x = x / (float)(width -	1) * (tile * width ? tile * width : 1);
			vertex.uv.y = z / (float)(height -	1) * (tile * height ? tile * height : 1);

			if (defaltpixels == true) {
				UINT index = width * z + x;
				vertex.pos.y = pixels[index].x * 20.0f * hight;
			}
			vertices.push_back(vertex);
		}
	}

	// Indices
	vector<UINT>& indices = mesh->GetIndices();

	indices.reserve((width - 1) * (height - 1) * 6);

	if (flip) 
	{
		for (UINT z = 0; z < height - 1; z++) {
			for (UINT x = 0; x < width - 1; x++) {
				indices.push_back(width * z + x + 1);	// 2
				indices.push_back(width * (z + 1) + x);	// 1
				indices.push_back(width * z + x);	// 0

				indices.push_back(width * (z + 1) + x + 1);	// 3
				indices.push_back(width * (z + 1) + x);	// 1
				indices.push_back(width * z + x + 1);	// 2
			}
		}
	}
	else
	{
		for (UINT z = 0; z < height - 1; z++) {
			for (UINT x = 0; x < width - 1; x++) {
				indices.push_back(width * z + x);	// 0
				indices.push_back(width * (z + 1) + x);	// 1
				indices.push_back(width * z + x + 1);	// 2

				indices.push_back(width * z + x + 1);	// 2
				indices.push_back(width * (z + 1) + x);	// 1
				indices.push_back(width * (z + 1) + x + 1);	// 3
			}
		}
	}
}

void Terrain::MakeNormal(bool Flip)
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

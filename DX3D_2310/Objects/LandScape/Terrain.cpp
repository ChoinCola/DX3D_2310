#include "Framework.h"

Terrain::Terrain()
{
	SetLocalPosition({ 0, 0, 0 });
	tag = "Terrain";
	material->SetShader(L"Light/SpecularLight.hlsl");
	material->SetDiffuseMap(L"Textures/Colors/White.png");

	heightMap = Texture::Add(L"Textures/HeightMaps/HeightMap.png");

	mesh = new Mesh<VertexUVNormal>();
	normalline = new Mesh<VertexColor>();
	RSset = new RasterizerState();

	MakeMesh();
	MakeNormal();
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
	SetRender();
	if (IsPrintNormalLine)
		normalline->Draw(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	if (IsMapLine)
	{
		RSset->CullMode(D3D11_CULL_NONE);
		RSset->FillMode(D3D11_FILL_WIREFRAME);
		RSset->SetState();
		ChackOne = true;
	}

	if (!IsMapLine) {
		RSset->CullMode(D3D11_CULL_BACK);
		RSset->FillMode(D3D11_FILL_SOLID);
		RSset->SetState();
		ChackOne = false;
	}

	mesh->Draw();
}

void Terrain::GUIRender()
{
	ImGui::Checkbox("Terrain_Print_NormalLine", &IsPrintNormalLine);
	ImGui::Checkbox("Terrain_Onley_Line", &IsMapLine);
	ImGui::Text("x: %d, z: %d, y:%d", x, z, y);
	ImGui::Text("normalize : x: %d, y: %d, z: %d", v.x, v.y, v.z);
	ImGui::Text("D : %d", D);

	__super::GUIRender();
}

Vector3 Terrain::GetOnGrondPosition(Vector3 ObjectPos)
{
	// 범위 밖 인경우 자기자신 반환
	// 좌표는 항상 맵위치의 상대위치로 계산한다.
	Vector3 LocalMaxPos =  Vector3(width, 0, height) + GetLocalPosition();
	Vector3 LocalLowPos = GetLocalPosition();

	if (ObjectPos.x > LocalMaxPos.x || ObjectPos.z > LocalMaxPos.z ||
		ObjectPos.x < LocalLowPos.x || ObjectPos.z < LocalLowPos.z)
		return ObjectPos;

	// 범위 안 일경우,
	// 현재 Object위치를 XY좌표로 반환하여 어떠한 사각형 안에 들어와있는지 판별함.
	// 기본 디폴트 칸 사이즈는 xy가 1임
	// 대충 xy좌표를 int로 변환하면 현재 Pos의 칸 내부가 나타남.
	vector<VertexType>& vertices = mesh->GetVertices();
	vector<UINT>& indices = mesh->GetIndices();

	x = ObjectPos.x;
	z = ObjectPos.z;

	// indicees를 참조해서 데이터를 구했으면, 3과 0중 더 가까운vertices를 찾아야함.
	// 사각형이 아닌 삼각형으로 vertices가 그어졌음으로 삼각형 기준으로 높이값을 구해야하기 때문.

	Vector3 e0 = vertices[width * z + x].pos;	// 0
	Vector3 e1 = vertices[width * (z + 1) + x].pos;	// 1
	Vector3 e2 = vertices[width * z + x + 1].pos;	// 2
	Vector3 e3 = vertices[width * (z + 1) + x + 1].pos;	// 3

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

		return Vector3(ObjectPos.x, y, ObjectPos.z);

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

		return Vector3(ObjectPos.x, y, ObjectPos.z);
	}

}

void Terrain::MakeMesh()
{
	width = heightMap->GetSize().x;
	height = heightMap->GetSize().y;

	vector<Float4> pixels;
	heightMap->ReadPixels(pixels);

	vector<VertexType>& vertices = mesh->GetVertices();

	vertices.reserve(width * height);
	for (UINT z = 0; z < height; z++) {
		for (UINT x = 0; x < width; x++) {
			VertexType vertex;
			vertex.pos = { (float)x, 0.0f, (float)z };
			vertex.uv.x = x / (float)(width - 1);
			vertex.uv.y = z / (float)(height - 1);

			UINT index = width * z + x;
			vertex.pos.y = pixels[index].x * 20.0f;

			vertices.push_back(vertex);
		}
	}

	// Indices
	vector<UINT>& indices = mesh->GetIndices();

	indices.reserve((width - 1) * (height - 1) * 6);

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

void Terrain::MakeNormal()
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

		vertices[index0].normal += noraml;
		vertices[index1].normal += noraml;
		vertices[index2].normal += noraml;
	}
}

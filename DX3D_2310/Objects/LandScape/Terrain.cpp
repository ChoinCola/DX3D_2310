#include "Framework.h"

Terrain::Terrain()
{
	SetLocalPosition({ 0, 0, 0 });
	tag = "Terrain";
	material->SetShader(L"Light/SpecularLight.hlsl");
	material->SetDiffuseMap(L"Textures/Colors/White.png");

	heightMap = Texture::Add(L"Textures/Coloes/Black.png");

	mesh = new Mesh<VertexUVNormal>();
	normalline = new Mesh<VertexColor>();
	RSset = new RasterizerState();

	MakeMesh();
	MakeNormal();
	mesh->CreateMesh();
	normalline->CreateMesh();
	RSset->SetState();
}

Terrain::Terrain(wstring HigntMap)
{
	SetLocalPosition({ 0, 0, 0 });
	tag = "Terrain";
	material->SetShader(L"Light/SpecularLight.hlsl");
	material->SetDiffuseMap(L"Textures/Colors/White.png");

	heightMap = Texture::Add(HigntMap);

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
	material->SetShader(L"Light/Light.hlsl");
	material->SetDiffuseMap(L"Textures/Colors/White.png");

	heightMap = Texture::Add(hightmap);

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

Vector3 Terrain::GetOnGrondPosition(const Vector3 ObjectPos, const Vector3 correction)
{
	// ���� �� �ΰ�� �ڱ��ڽ� ��ȯ
	// ��ǥ�� �׻� ����ġ�� �����ġ�� ����Ѵ�.
	Vector3 LocalMaxPos =  Vector3(width, 0, height) + GetLocalPosition();
	Vector3 LocalLowPos = GetLocalPosition();
	x = (int)ObjectPos.x;
	z = (int)ObjectPos.z;

	if (x < 0 || x >= width - 1)		return ObjectPos;
	if (z < 0 || z >= height - 1)		return ObjectPos;

	// ���� �� �ϰ��,
	// ���� Object��ġ�� XY��ǥ�� ��ȯ�Ͽ� ��� �簢�� �ȿ� �����ִ��� �Ǻ���.
	// �⺻ ����Ʈ ĭ ������� xy�� 1��
	// ���� xy��ǥ�� int�� ��ȯ�ϸ� ���� Pos�� ĭ ���ΰ� ��Ÿ��.
	vector<VertexType>& vertices = mesh->GetVertices();
	vector<UINT>& indices = mesh->GetIndices();


	// indicees�� �����ؼ� �����͸� ��������, 3�� 0�� �� �����vertices�� ã�ƾ���.
	// �簢���� �ƴ� �ﰢ������ vertices�� �׾��������� �ﰢ�� �������� ���̰��� ���ؾ��ϱ� ����.
	UINT index[4];
	index[0] = width * z + x;
	index[1] = width * (z + 1) + x;
	index[2] = width * z + x + 1;
	index[3] = width * (z + 1) + x + 1;

	Vector3 e0 = vertices[index[0]].pos;	// 0
	Vector3 e1 = vertices[index[1]].pos;	// 1
	Vector3 e2 = vertices[index[2]].pos;	// 2
	Vector3 e3 = vertices[index[3]].pos;	// 3

	// ��Į���� ������. e0�� �� �������, e0���� ����
	if (ObjectPos.x <  ObjectPos.z)
	{
		// e0e1, e0e2 ����.
		v = Vector3::ComputeNormal(e0, e1, e2);
		//��������,
		// x + y + z + D = 0;
		// D ���ϱ�.
		// D = -Ax1 - By1 - Cz1; �������� �������� �Ÿ�
		// D = -v.x * e0.x + -v.y * e0.y + -v.z * e0.z
		D = -v.x * e0.x - v.y * e0.y - v.z * e0.z;
		// ���� ������. y = - x - z - D
		y = -(v.x * ObjectPos.x + v.z * ObjectPos.z + D) / v.y;

		return Vector3(ObjectPos.x, y, ObjectPos.z) + correction;

	}
	// e3�� �� �����ų� �������, e3�� ����.
	else
	{
		// e0e1, e0e2 ����.
		v = Vector3::ComputeNormal(e3, e1, e2);
		//��������,
		// x + y + z + D = 0;
		// D ���ϱ�.
		// D = -Ax1 - By1 - Cz1;
		// D = -v.x * e0.x + -v.y * e0.y + -v.z * e0.z
		D = -v.x * e3.x - v.y * e3.y - v.z * e3.z;
		// ���� ������. y = - x - z - D
		y = -(v.x * ObjectPos.x + v.z * ObjectPos.z + D) / v.y;

		return Vector3(ObjectPos.x, y, ObjectPos.z) + correction;
	}

}
//Vector3 Terrain::GetOnGrondPosition(const Vector3 ObjectPos)
//{
//	int x = (int)ObjectPos.x;
//	int z = (int)ObjectPos.z;
//
//	if (x < 0 || x >= width - 1) return ObjectPos;
//	if (z < 0 || z >= height - 1) return ObjectPos;
//
//	UINT index[4];
//	index[0] = width * z + x;
//	index[1] = width * (z + 1) + x;
//	index[2] = width * z + x + 1;
//	index[3] = width * (z + 1) + x + 1;
//
//	vector<VertexType>& vertices = mesh->GetVertices();
//
//	Vector3 p[4];
//	FOR(4)
//		p[i] = vertices[index[i]].pos;
//
//	float u = ObjectPos.x - p[0].x;
//	float v = ObjectPos.z - p[0].z;
//
//	Vector3 result;
//	
//	if (u + v <= 1.0f) {
//		result = ((p[2] - p[0]) * u + (p[1] - p[0]) * v) + p[0];
//	}
//	else
//	{
//		u = 1.0f - u;
//		v = 1.0f - v;
//
//		result = ((p[1] - p[3]) * u + (p[2] - p[3]) * v) + p[3];
//	}
//
//	return result;
//
//}

void Terrain::MakeMesh(bool tile, bool flip)
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
			vertex.uv.x = x / (float)(width -	1) * (tile * width ? tile * width : 1);
			vertex.uv.y = z / (float)(height -	1) * (tile * height ? tile * height : 1);

			UINT index = width * z + x;
			vertex.pos.y = pixels[index].x * 20.0f * hight;

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
